/**
* @file    TextParticle.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Text particle implementation.
*/

#include "animation/particles/TextParticle.h"

#include "sdl/resources.h"

namespace questless
{
	sdl::FontHandle TextParticle::_font_handle;

	Initializer<TextParticle> TextParticle::_initializer;
	void TextParticle::initialize()
	{
		_font_handle = sdl::font_manager().add("resources/fonts/firamono.ttf", 20);
	}

	sdl::Texture TextParticle::make_texture() const
	{
		return sdl::font_manager()[_font_handle].render(_text.c_str(), _color);
	}
}
