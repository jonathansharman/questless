/**
* @file    ItemTexturer.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the ItemTexturer class.
*/

#ifndef ITEM_TEXTURER_H
#define ITEM_TEXTURER_H

#include "items/ItemVisitor.h"
#include "sdl-wrappers/Texture.h"

namespace questless
{
	class ItemTexturer : public ItemVisitor
	{
	public:
		void visit(const Scroll&) override;
		void visit(const Quarterstaff&) override;

		sdl::Texture::ptr texture() { return std::move(_texture); }
	private:
		sdl::Texture::ptr _texture;
	};
}

#endif
