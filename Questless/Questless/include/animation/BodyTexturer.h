/**
* @file    BodyTexturer.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the BodyTexturer class.
*/

#pragma once

#include "sdl/Texture.h"
#include "entities/beings/Body.h"

namespace questless
{
	class BodyTexturer
	{
	public:
		void visit(Body const& body);

		sdl::Texture::uptr texture() { return std::move(_texture); }
	private:
		sdl::Texture::uptr _texture;
	};
}
