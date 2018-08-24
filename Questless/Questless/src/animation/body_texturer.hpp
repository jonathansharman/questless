//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "sdl/texture.hpp"
#include "utility/reference.hpp"

namespace ql {
	class body;

	//! Creates textures for bodies.
	class body_texturer {
	public:
		void visit(body const& body);

		uptr<sdl::texture> texture() { return std::move(_texture); }
	private:
		uptr<sdl::texture> _texture;
	};
}
