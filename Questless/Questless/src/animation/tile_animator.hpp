//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/reference.hpp"
#include "world/tile_visitor.hpp"

namespace ql {
	namespace rsrc {
		struct tile;
	}
	class animation;

	//! Creates animations for tiles.
	struct tile_animator : public tile_const_visitor {
		rsrc::tile const& resources;

		uptr<animation> animation;

		tile_animator(rsrc::tile const& resources) : resources{resources} {}

		~tile_animator();

		void visit(dirt_tile const&) final;
		void visit(edge_tile const&) final;
		void visit(grass_tile const&) final;
		void visit(sand_tile const&) final;
		void visit(snow_tile const&) final;
		void visit(stone_tile const&) final;
		void visit(water_tile const&) final;
	};
}
