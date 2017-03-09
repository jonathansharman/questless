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
		void visit(Scroll const&) override;
		void visit(Quarterstaff const&) override;
		void visit(Bow const&) override;

		sdl::Texture::ptr texture() { return std::move(_texture); }
	private:
		sdl::Texture::ptr _texture;
	};
}
