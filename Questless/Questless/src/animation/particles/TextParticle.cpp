/**
* @file    TextParticle.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Text particle implementation.
*/

#include "animation/particles/TextParticle.h"
#include "sdl-wrappers/resources.h"

using namespace sdl;

namespace questless
{
	Handle<Font> TextParticle::_font_handle;

	Initializer<TextParticle> TextParticle::_initializer;
	void TextParticle::initialize()
	{
		_font_handle = font_manager().add([] {
			return Font::make("resources/fonts/firamono.ttf", 20, SDL_BLENDMODE_BLEND);
		});
	}

	Texture TextParticle::make_texture() const
	{
		return font_manager()[_font_handle].render(_text, renderer(), _color);
	}
}
