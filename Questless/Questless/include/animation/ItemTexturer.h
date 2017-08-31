//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/ItemVisitor.h"
#include "sdl/resources.h"
#include "utility/reference.h"

namespace questless
{
	//! Creates textures for items.
	class ItemTexturer : public ItemConstVisitor
	{
	public:
		void visit(Arrow const&) final;
		void visit(Bow const&) final;
		void visit(Quarterstaff const&) final;
		void visit(Quiver const&) final;
		void visit(Scroll const&) final;
		void visit(Gatestone const&) final;

		//! A handle to the texture of the last visited item.
		auto texture_handle() const { return _texture_handle; }
	private:
		sdl::TextureHandle _texture_handle;
	};
}
