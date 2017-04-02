/**
* @file    ItemTexturer.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the ItemTexturer class.
*/

#pragma once

#include "items/ItemVisitor.h"
#include "sdl/Texture.h"

namespace questless
{
	class ItemTexturer : public ItemVisitor
	{
	public:
		void visit(Arrow const&) override;
		void visit(Bow const&) override;
		void visit(Quarterstaff const&) override;
		void visit(Quiver const&) override;
		void visit(Scroll const&) override;

		sdl::Texture::uptr texture() { return std::move(_texture); }
	private:
		sdl::Texture::uptr _texture;
	};
}
