//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "magic/spell_visitor.hpp"
#include "sdl/resources.hpp"
#include "utility/reference.hpp"

namespace ql
{
	//! Creates textures for items.
	class spell_texturer : public magic::spell_const_visitor
	{
	public:
		void visit(magic::eagle_eye const&) final;
		void visit(magic::heal const&) final;
		void visit(magic::shock const&) final;
		void visit(magic::teleport const&) final;

		//! A handle to the texture of the last visited item.
		auto texture_handle() const { return _texture_handle; }
	private:
		sdl::texture_handle _texture_handle;
	};
}
