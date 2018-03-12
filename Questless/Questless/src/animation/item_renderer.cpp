//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include <unordered_map>

#include <boost/functional/hash.hpp>

#include "animation/item_renderer.hpp"
#include "animation/spell_texturer.hpp"
#include "sdl/resources.hpp"
#include "items/magic/gatestone.hpp"
#include "items/scroll.hpp"

using namespace sdl;
using namespace units;

namespace ql
{
	void item_renderer::visit(arrow const&)
	{
		static auto texture_handle = the_texture_manager().add("resources/textures/items/arrow.png");
		the_texture_manager()[texture_handle].draw(_position);
	}

	void item_renderer::visit(bow const&)
	{
		static auto texture_handle = the_texture_manager().add("resources/textures/items/bow.png");
		the_texture_manager()[texture_handle].draw(_position);
	}

	void item_renderer::visit(quarterstaff const&)
	{
		static auto texture_handle = the_texture_manager().add("resources/textures/items/quarterstaff.png");
		the_texture_manager()[texture_handle].draw(_position);
	}

	void item_renderer::visit(quiver const&)
	{
		static auto texture_handle = the_texture_manager().add("resources/textures/items/quiver.png");
		the_texture_manager()[texture_handle].draw(_position);
	}

	void item_renderer::visit(scroll const& scroll)
	{
		if (scroll.blank()) {
			static auto texture_handle = the_texture_manager().add("resources/textures/items/blank-scroll.png");
			the_texture_manager()[texture_handle].draw(_position);
		} else {
			static auto scroll_handle = the_texture_manager().add("resources/textures/items/uncharged-scroll.png");

			magic::spell const& spell = scroll.spell();

			// Get the spell texture.
			spell_texturer spell_texturer;
			spell.accept(spell_texturer);
			auto spell_handle = spell_texturer.texture_handle();

			// Static cache to hold previously computed texture handles.
			using handle_t = handle<texture>;
			static std::unordered_map<std::pair<handle_t, handle_t>, handle_t, boost::hash<std::pair<handle_t, handle_t>>> handle_cache;

			// The current scroll and spell handles together form the key for the cache.
			auto scroll_and_spell_handle = std::make_pair(scroll_handle, spell_handle);

			// Cache lookup.
			auto it = handle_cache.find(scroll_and_spell_handle); //! @todo Use initializer if here when available.
			if (it != handle_cache.end()) {
				// Cache hit. Return previously computed handle.
				the_texture_manager()[it->second].draw(_position);
			} else {
				// Cache miss. Create handle to texture formed by pasting the spell texture on top of the scroll texture.
				auto texture_handle = the_texture_manager().add_with_generator([=] {
					texture& scroll_texture = the_texture_manager()[scroll_handle];
					auto texture = umake<sdl::texture>(scroll_texture.size());
					texture->as_target([&] {
						scroll_texture.draw(window_space::point{0, 0});
						the_texture_manager()[spell_handle].draw(window_space::point{0, 0});
					});
					return texture;
				});

				// Cache and return the resulting handle.
				handle_cache.insert(std::make_pair(scroll_and_spell_handle, texture_handle));
				the_texture_manager()[texture_handle].draw(_position);
			}
		}
	}

	void item_renderer::visit(gatestone const& gatestone)
	{
		static auto empty_texture_handle = the_texture_manager().add("resources/textures/items/soul-gem-empty.png");
		if (gatestone.mana == 0.0) {
			the_texture_manager()[empty_texture_handle].draw(_position);
		} else {
			static auto non_empty_texture_handle = the_texture_manager().add("resources/textures/items/soul-gem.png");
			colors::color_vector const draw_color_vector = [&] {
				float const alpha = static_cast<float>(gatestone.mana / gatestone.capacity());
				switch (gatestone.color()) {
					case magic::color::white:  return colors::white_vector(alpha);
					case magic::color::black:  return colors::color_vector{0.2f, 0.2f, 0.2f, alpha};
					case magic::color::green:  return colors::green_vector(alpha);
					case magic::color::red:    return colors::red_vector(alpha);
					case magic::color::blue:   return colors::blue_vector(alpha);
					case magic::color::yellow: return colors::yellow_vector(alpha);
					case magic::color::violet: return colors::purple_vector(alpha);
					case magic::color::orange: return colors::orange_vector(alpha);
					default: throw std::logic_error{"Unknown spell color."};
				}
			}();
			// Draw the gatestone.
			the_texture_manager()[empty_texture_handle].draw(_position, texture_space::align_left, texture_space::align_top);
			the_texture_manager()[non_empty_texture_handle].draw(_position, texture_space::align_left, texture_space::align_top, draw_color_vector);
			// Draw charge bar.
			the_renderer().draw_box(window_space::box{_position, window_space::vector{6, 55}}, colors::black());
			colors::color const fill_color{draw_color_vector.red(), draw_color_vector.green(), draw_color_vector.blue(), draw_color_vector.alpha()};
			the_renderer().draw_box(window_space::box{_position, window_space::vector{6, static_cast<int>(55 * gatestone.mana / gatestone.capacity())}}, 1, colors::black(), fill_color);

			//! @todo Clean this up. Remove magic numbers.
		}
	}
}
