//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "sdl/Texture.h"
#include "entities/beings/Body.h"

namespace questless
{
	//! Creates textures for bodies.
	class BodyTexturer
	{
	public:
		void visit(Body const& body);

		sdl::Texture::uptr texture() { return std::move(_texture); }
	private:
		sdl::Texture::uptr _texture;
	};
}
