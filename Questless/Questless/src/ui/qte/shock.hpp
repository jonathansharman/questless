//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "ui/dialog.hpp"

#include "quantities/wall_time.hpp"
#include "world/coordinates.hpp"

namespace ql::qte {
	//! Quick time event for determining shock strength.
	class shock : public dialog {
	public:
		//! @param target_coords The target strike coordinates.
		//! @param cont The dialog continuation function.
		shock(region_tile::point target_coords, std::function<void(double)> cont);

		state update() final;

		void draw() const final;
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

		uptr<sdl::texture> _txt_title;
		uptr<sdl::texture> _txt_prompt;
	};
}
