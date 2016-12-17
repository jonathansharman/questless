/**
* @file    WhiteMagic.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the WhiteMagic particle class.
*/

#include "animation/particles/WhiteMagic.h"
#include "sdl-wrappers/resources.h"

using namespace sdl;

namespace questless
{
	void WhiteMagic::subupdate()
	{
		_velocity.step().y += _gravity * frame_duration;
		_velocity.step().x *= _vx_factor_per_frame;
	}

	Texture& WhiteMagic::texture() const
	{
		static bool first_call = true;
		static Handle<Texture> handle;
		if (first_call) {
			handle = texture_manager().add([] {
				return Texture::make("resources/textures/particles/magic/white.png", renderer(), SDL_BLENDMODE_BLEND);
			});
			first_call = false;
		}
		return texture_manager()[handle];
	}
}