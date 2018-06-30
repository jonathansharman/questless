//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "ui/dialog.hpp"
#include "world/coordinates.hpp"

namespace ql::qte
{
	//! Quick time event for determining shock strength.
	class shock : public dialog
	{
	public:
		//! @param target_coords The target strike coordinates.
		//! @param cont The dialog continuation function.
		shock(region_tile::point target_coords, std::function<void(double)> cont);

		state update() final;

		void draw() const final;
	private:
		struct charge
		{
			units::game_space::point position;
			units::game_space::velocity velocity;
		};

		units::game_space::point _target_point;
		continuation<double> _cont;

		units::game_space::seconds _elapsed_time = 0.0s;
		int _quadrant = 0;

		std::vector<charge> _charges;

		uptr<sdl::texture> _txt_title;
		uptr<sdl::texture> _txt_prompt;
	};
}
