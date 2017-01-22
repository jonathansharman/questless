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
#include "sdl-wrappers/Texture.h"

namespace questless
{
	class TileTexturer : public TileVisitor
	{
	public:
		void visit(const EdgeTile&) override;
		void visit(const StoneTile&) override;
		void visit(const DirtTile&) override;
		void visit(const GrassTile&) override;
		void visit(const WaterTile&) override;
		void visit(const SnowTile&) override;

		sdl::Texture::ptr texture() { return std::move(_texture); }
	private:
		sdl::Texture::ptr _texture;
	};
}
