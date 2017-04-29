//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "world/Tile.h"
#include "sdl/Texture.h"
#include "utility/reference.h"

namespace questless
{
	//! Creates textures for tiles.
	class TileTexturer : public TileConstVisitor
	{
	public:
		void visit(DirtTile const&) override;
		void visit(EdgeTile const&) override;
		void visit(GrassTile const&) override;
		void visit(SandTile const&) override;
		void visit(SnowTile const&) override;
		void visit(StoneTile const&) override;
		void visit(WaterTile const&) override;

		uptr<sdl::Texture> texture() { return std::move(_texture); }
	private:
		uptr<sdl::Texture> _texture;
	};
}
