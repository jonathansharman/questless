//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include <unordered_map>

#include <boost/functional/hash.hpp>

#include "animation/ItemRenderer.h"
#include "animation/SpellTexturer.h"
#include "sdl/resources.h"
#include "items/magic/Gatestone.h"
#include "items/Scroll.h"

using namespace sdl;
using namespace units;

namespace questless
{
	void ItemRenderer::visit(Arrow const&)
	{
		static auto texture_handle = texture_manager().add("resources/textures/items/arrow.png");
		texture_manager()[texture_handle].draw(_position);
	}

	void ItemRenderer::visit(Bow const&)
	{
		static auto texture_handle = texture_manager().add("resources/textures/items/bow.png");
		texture_manager()[texture_handle].draw(_position);
	}

	void ItemRenderer::visit(Quarterstaff const&)
	{
		static auto texture_handle = texture_manager().add("resources/textures/items/quarterstaff.png");
		texture_manager()[texture_handle].draw(_position);
	}

	void ItemRenderer::visit(Quiver const&)
	{
		static auto texture_handle = texture_manager().add("resources/textures/items/quiver.png");
		texture_manager()[texture_handle].draw(_position);
	}

	void ItemRenderer::visit(Scroll const& scroll)
	{
		if (scroll.blank()) {
			static auto texture_handle = texture_manager().add("resources/textures/items/blank-scroll.png");
			texture_manager()[texture_handle].draw(_position);
		} else {
			static auto scroll_handle = texture_manager().add("resources/textures/items/uncharged-scroll.png");

			spell::Spell const& spell = scroll.spell();

			// Get the spell texture.
			SpellTexturer spell_texturer;
			spell.accept(spell_texturer);
			auto spell_handle = spell_texturer.texture_handle();

			// Static cache to hold previously computed texture handles.
			using handle_t = Handle<Texture>;
			static std::unordered_map<std::pair<handle_t, handle_t>, handle_t, boost::hash<std::pair<handle_t, handle_t>>> handle_cache;

			// The current scroll and spell handles together form the key for the cache.
			auto scroll_and_spell_handle = std::make_pair(scroll_handle, spell_handle);

			// Cache lookup.
			auto it = handle_cache.find(scroll_and_spell_handle); //! @todo Use initializer if here when available.
			if (it != handle_cache.end()) {
				// Cache hit. Return previously computed handle.
				texture_manager()[it->second].draw(_position);
			} else {
				// Cache miss. Create handle to texture formed by pasting the spell texture on top of the scroll texture.
				auto texture_handle = texture_manager().add_with_generator([=] {
					Texture& scroll_texture = texture_manager()[scroll_handle];
					auto texture = std::make_unique<Texture>(scroll_texture.width(), scroll_texture.height());
					texture->as_target([&] {
						scroll_texture.draw(ScreenSpace::Point{0, 0});
						texture_manager()[spell_handle].draw(ScreenSpace::Point{0, 0});
					});
					return texture;
				});

				// Cache and return the resulting handle.
				handle_cache.insert(std::make_pair(scroll_and_spell_handle, texture_handle));
				texture_manager()[texture_handle].draw(_position);
			}
		}
	}

	void ItemRenderer::visit(Gatestone const& gatestone)
	{
		if (gatestone.mana == 0.0) {
			static auto texture_handle = texture_manager().add("resources/textures/items/soul-gem-empty.png");
			texture_manager()[texture_handle].draw(_position);
		} else {
			static auto texture_handle = texture_manager().add("resources/textures/items/soul-gem-white.png");
			colors::ColorFactor draw_color_factor;
			switch (gatestone.color()) {
				case spell::Color::white:
					draw_color_factor = colors::white_factor();
					break;
				case spell::Color::black:
					draw_color_factor = colors::ColorFactor{0.2f, 0.2f, 0.2f, 1.0f};
					break;
				case spell::Color::green:
					draw_color_factor = colors::green_factor();
					break;
				case spell::Color::red:
					draw_color_factor = colors::red_factor();
					break;
				case spell::Color::blue:
					draw_color_factor = colors::blue_factor();
					break;
				case spell::Color::yellow:
					draw_color_factor = colors::yellow_factor();
					break;
				case spell::Color::violet:
					draw_color_factor = colors::purple_factor();
					break;
				case spell::Color::orange:
					draw_color_factor = colors::orange_factor();
					break;
			}
			texture_manager()[texture_handle].draw(_position, TextureSpace::align_left, TextureSpace::align_top, draw_color_factor);
			colors::Color fill_color{draw_color_factor.red(), draw_color_factor.green(), draw_color_factor.blue(), draw_color_factor.alpha()};
			renderer().draw_box(ScreenSpace::Box{_position, ScreenSpace::Vector{6, 55}}, colors::black(), sdl::Fill::solid);
			renderer().draw_box(ScreenSpace::Box{_position, ScreenSpace::Vector{6, static_cast<int>(55 * gatestone.mana / gatestone.capacity())}}, colors::black(), fill_color);

			//! @todo Clean this up. Remove magic numbers.
		}
	}
}
