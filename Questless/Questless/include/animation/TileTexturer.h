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
		void visit(DirtTile const&) final;
		void visit(EdgeTile const&) final;
		void visit(GrassTile const&) final;
		void visit(SandTile const&) final;
		void visit(SnowTile const&) final;
		void visit(StoneTile const&) final;
		void visit(WaterTile const&) final;

		uptr<sdl::Texture> texture() { return std::move(_texture); }
	private:
		uptr<sdl::Texture> _texture;
	};
}
