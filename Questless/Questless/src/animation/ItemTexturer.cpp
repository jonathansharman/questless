//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include <unordered_map>

#include <boost/functional/hash.hpp>

#include "animation/ItemTexturer.h"
#include "animation/SpellTexturer.h"
#include "sdl/resources.h"
#include "items/Scroll.h"

using namespace sdl;
using namespace units;

namespace questless
{
	void ItemTexturer::visit(Arrow const&)
	{
		static auto texture_handle = texture_manager().add("resources/textures/items/arrow.png");
		_texture_handle = texture_handle;
	}

	void ItemTexturer::visit(Bow const&)
	{
		static auto texture_handle = texture_manager().add("resources/textures/items/bow.png");
		_texture_handle = texture_handle;
	}

	void ItemTexturer::visit(Quarterstaff const&)
	{
		static auto texture_handle = texture_manager().add("resources/textures/items/quarterstaff.png");
		_texture_handle = texture_handle;
	}

	void ItemTexturer::visit(Quiver const&)
	{
		static auto texture_handle = texture_manager().add("resources/textures/items/quiver.png");
		_texture_handle = texture_handle;
	}

	void ItemTexturer::visit(Scroll const& scroll)
	{
		if (scroll.blank()) {
			static auto texture_handle = texture_manager().add("resources/textures/items/blank-scroll.png");
			_texture_handle = texture_handle;
		} else {
			static auto uncharged_handle = texture_manager().add("resources/textures/items/uncharged-scroll.png");
			static auto charged_handle = texture_manager().add("resources/textures/items/charged-scroll.png");

			spell::Spell const& spell = scroll.spell();

			// Get the underlying scroll texture.
			auto scroll_handle = spell.max_charges() && spell.charges() > 0 ? charged_handle : uncharged_handle;

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
				_texture_handle = it->second;
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
				_texture_handle = texture_handle;
			}
		}
	}
}
