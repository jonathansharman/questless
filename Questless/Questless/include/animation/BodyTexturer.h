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

		uptr<sdl::Texture> texture() { return std::move(_texture); }
	private:
		uptr<sdl::Texture> _texture;
	};
}
