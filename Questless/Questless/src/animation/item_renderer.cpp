//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "item_renderer.hpp"

#include <unordered_map>

#include <boost/functional/hash.hpp>

#include "spell_texturer.hpp"

#include "sdl/resources.hpp"
#include "items/magic/gatestone.hpp"
#include "items/scroll.hpp"

using namespace sdl;
using namespace sdl::spaces::window::literals;
using namespace sdl::spaces::colors::literals;

namespace ql {
	void item_renderer::visit(arrow const&) {
		static auto texture_handle = the_texture_manager().add("resources/textures/items/arrow.png");
		the_texture_manager()[texture_handle].draw(_position);
	}

	void item_renderer::visit(bow const&) {
		static auto texture_handle = the_texture_manager().add("resources/textures/items/bow.png");
		the_texture_manager()[texture_handle].draw(_position);
	}

	void item_renderer::visit(quarterstaff const&) {
		static auto texture_handle = the_texture_manager().add("resources/textures/items/quarterstaff.png");
		the_texture_manager()[texture_handle].draw(_position);
	}

	void item_renderer::visit(quiver const&) {
		static auto texture_handle = the_texture_manager().add("resources/textures/items/quiver.png");
		the_texture_manager()[texture_handle].draw(_position);
	}

	void item_renderer::visit(scroll const& scroll) {
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
						scroll_texture.draw(spaces::window::point{0_px, 0_px});
						the_texture_manager()[spell_handle].draw(spaces::window::point{0_px, 0_px});
					});
					return texture;
				});

				// Cache and return the resulting handle.
				handle_cache.insert(std::make_pair(scroll_and_spell_handle, texture_handle));
				the_texture_manager()[texture_handle].draw(_position);
			}
		}
	}

	void item_renderer::visit(gatestone const& gatestone) {
		static auto empty_texture_handle = the_texture_manager().add("resources/textures/items/soul-gem-empty.png");
		if (gatestone.charge == 0.0_mp) {
			the_texture_manager()[empty_texture_handle].draw(_position);
		} else {
			static auto non_empty_texture_handle = the_texture_manager().add("resources/textures/items/soul-gem.png");
			spaces::colors::color const draw_color_vector = [&] {
				spaces::colors::part const alpha{static_cast<float>((gatestone.charge.value() / gatestone.capacity()).value)};
				switch (gatestone.color()) {
					case magic::color::white:  return spaces::colors::white(alpha);
					case magic::color::black:  return spaces::colors::color{0.2_c, 0.2_c, 0.2_c, alpha};
					case magic::color::green:  return spaces::colors::green(alpha);
					case magic::color::red:    return spaces::colors::red(alpha);
					case magic::color::blue:   return spaces::colors::blue(alpha);
					case magic::color::yellow: return spaces::colors::yellow(alpha);
					case magic::color::violet: return spaces::colors::purple(alpha);
					case magic::color::orange: return spaces::colors::orange(alpha);
					default: assert(false && "Invalid spell color."); //! @todo Replace with precondition when available and remove dummy return value.
				}
				return spaces::colors::color{};
			}();
			// Draw the gatestone.
			the_texture_manager()[empty_texture_handle].draw(_position, spaces::window::left_align, spaces::window::top_align);
			the_texture_manager()[non_empty_texture_handle].draw(_position, spaces::window::left_align, spaces::window::top_align, draw_color_vector);
			// Draw charge bar.
			the_renderer().draw_box(spaces::window::box{_position, {6_px, 55_px}}, spaces::colors::black());
			spaces::colors::color const fill_color{r(draw_color_vector), g(draw_color_vector), b(draw_color_vector), 1.0_c};
			the_renderer().draw_box
				( spaces::window::box
					{ _position
					, spaces::window::vector{6_px, spaces::window::px{static_cast<int>(55.0 * (gatestone.charge.value() / gatestone.capacity()).value)}}
					}
				, 1_px
				, spaces::colors::black()
				, fill_color
				);

			//! @todo Clean this up. Remove magic numbers.
		}
	}
}
