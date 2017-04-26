//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/ItemVisitor.h"
#include "sdl/Texture.h"
#include "utility/reference.h"

namespace questless
{
	//! Creates textures for items.
	class ItemTexturer : public ItemVisitor
	{
	public:
		void visit(Arrow const&) override;
		void visit(Bow const&) override;
		void visit(Quarterstaff const&) override;
		void visit(Quiver const&) override;
		void visit(Scroll const&) override;

		uptr<sdl::Texture> texture() { return std::move(_texture); }
	private:
		uptr<sdl::Texture> _texture;
	};
}
