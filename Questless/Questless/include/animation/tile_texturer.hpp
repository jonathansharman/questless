//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "world/tile_visitor.hpp"
#include "sdl/texture.hpp"
#include "utility/reference.hpp"

namespace ql
{
	//! Creates textures for tiles.
	class tile_texturer : public tile_const_visitor
	{
	public:
		void visit(dirt_tile const&) final;
		void visit(edge_tile const&) final;
		void visit(grass_tile const&) final;
		void visit(sand_tile const&) final;
		void visit(snow_tile const&) final;
		void visit(stone_tile const&) final;
		void visit(water_tile const&) final;

		uptr<sdl::texture> texture() { return std::move(_texture); }
	private:
		uptr<sdl::texture> _texture;
	};
}
