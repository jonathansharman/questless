/**
* @file    BodyTexturer.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the BodyTexturer class.
*/

#ifndef BODY_TEXTURER_H
#define BODY_TEXTURER_H

#include "sdl-wrappers/Texture.h"
#include "entities/beings/Body.h"

namespace questless
{
	class BodyTexturer
	{
	public:
		void visit(const Body&);

		sdl::Texture::ptr texture() { return std::move(_texture); }
	private:
		sdl::Texture::ptr _texture;
	};
}

#endif
