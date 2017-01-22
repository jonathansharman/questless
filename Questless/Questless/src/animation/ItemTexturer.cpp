/**
* @file    ItemTexturer.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the ItemTexturer class.
*/

#include "animation/ItemTexturer.h"
#include "sdl-wrappers/resources.h"
#include "items/Scroll.h"

using std::make_unique;

using sdl::Texture;
using sdl::renderer;

namespace questless
{
	void ItemTexturer::visit(Scroll const& scroll)
	{
		if (scroll.blank()) {
			_texture = make_unique<Texture>("resources/textures/items/blank-scroll.png", renderer());
		} else {
			spell::Spell const& spell = scroll.spell();
			if (spell.max_charges() && spell.charges() > 0) {
				_texture = make_unique<Texture>("resources/textures/items/charged-scroll.png", renderer());
			} else {
				_texture = make_unique<Texture>("resources/textures/items/uncharged-scroll.png", renderer());
			}
		}
	}

	void ItemTexturer::visit(Quarterstaff const&)
	{
		_texture = make_unique<Texture>("resources/textures/items/quarterstaff.png", renderer());
	}
}
