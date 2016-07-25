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

using sdl::Texture;
using sdl::renderer;

namespace questless
{
	void ItemTexturer::visit(const Scroll& scroll)
	{
		if (scroll.blank()) {
			_texture = Texture::make("resources/textures/items/blank-scroll.png", renderer());
		} else {
			const Spell& spell = scroll.spell();
			if (spell.max_charges() && spell.charges() > 0) {
				_texture = Texture::make("resources/textures/items/charged-scroll.png", renderer());
			} else {
				_texture = Texture::make("resources/textures/items/uncharged-scroll.png", renderer());
			}
		}
	}

	void ItemTexturer::visit(const Quarterstaff&)
	{
		_texture = Texture::make("resources/textures/items/quarterstaff.png", renderer());
	}
}
