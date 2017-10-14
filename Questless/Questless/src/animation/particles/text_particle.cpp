//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/particles/text_particle.h"

#include "sdl/resources.h"

namespace ql
{
	sdl::font_handle text_particle::_font_handle;

	initializer<text_particle> text_particle::_initializer;
	void text_particle::initialize()
	{
		_font_handle = sdl::the_font_manager().add("resources/fonts/firamono.ttf", 20);
	}

	sdl::texture text_particle::make_texture() const
	{
		return sdl::the_font_manager()[_font_handle].render(_text.c_str(), _color);
	}
}
