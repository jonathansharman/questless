/**
* @file    TileTexturer.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the TileTexturer class.
*/

#pragma once

#include "world/Tile.h"
#include "sdl/Texture.h"

namespace questless
{
	class TileTexturer : public TileVisitor
	{
	public:
		void visit(DirtTile const&) override;
		void visit(EdgeTile const&) override;
		void visit(GrassTile const&) override;
		void visit(SandTile const&) override;
		void visit(SnowTile const&) override;
		void visit(StoneTile const&) override;
		void visit(WaterTile const&) override;

		sdl::Texture::uptr texture() { return std::move(_texture); }
	private:
		sdl::Texture::uptr _texture;
	};
}
