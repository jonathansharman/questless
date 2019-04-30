//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "ui/dialog.hpp"

#include "quantities/wall_time.hpp"
#include "world/coordinates.hpp"

namespace ql::qte {
	//! Quick time event for determining shock strength.
	struct shock : dialog {
		//! @param target_coords The target strike coordinates.
		//! @param cont The dialog continuation function.
		shock(sf::Window const& window, rsrc::fonts const& fonts, region_tile::point target_coords, std::function<void(double)> cont);

		void update(sec elapsed_time, input_manager& im) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

	private:
		struct charge {
			world::point position;
			world::vel velocity;
		};

		world::point _target_point;
		continuation<double> _cont;

		sec _elapsed_time = 0.0_s;
		enum class quadrant { ur, ul, ll, lr } _quadrant = quadrant::ur;

		std::vector<charge> _charges;

		sf::Text _title;
		sf::Text _prompt;
	};
}
