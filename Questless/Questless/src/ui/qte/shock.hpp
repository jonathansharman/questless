//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "ui/dialog/dialog.hpp"

#include "quantities/wall_time.hpp"
#include "rsrc/particle_fwd.hpp"
#include "world/coordinates.hpp"

namespace ql {
	namespace qte {
		//! Quick time event for determining shock strength.
		struct shock : dialog<float> {
			//! @param target_coords The target strike coordinates.
			shock(rsrc::particle const& particle_resources, tile_hex_point target_coords);

			auto update(sec elapsed_time) -> void final;

			auto on_mouse_move(view::point mouse_position) -> void final;

		private:
			struct charge {
				view::point position;
				view::vel velocity;
			};

			rsrc::particle_ptr _particle_resources;

			view::point _target_point;

			sec _elapsed_time = 0.0_s;
			enum class quadrant { ur, ul, ll, lr } _quadrant = quadrant::ur;

			std::vector<charge> _charges;

			auto draw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;
		};
	}
}
