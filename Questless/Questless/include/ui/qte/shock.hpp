//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <vector>
#include <functional>

#include "../dialog.hpp"
#include "animation/camera.hpp"
#include "units/game_space.hpp"

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
		static constexpr units::game_space::seconds _time_limit = 5.0s;
		static constexpr int _charges_per_quadrant = 4;
		static constexpr double _expected_charges = _charges_per_quadrant * 70.0;

		struct charge
		{
			units::game_space::point position;
			units::game_space::velocity velocity;
		};

		static sdl::texture_handle _point_charge_texture_handle;

		units::game_space::point _target_point;
		continuation<double> _cont;

		units::game_space::seconds _elapsed_time = 0.0s;
		int _quadrant = 0;

		std::vector<charge> _charges;

		uptr<sdl::texture> _txt_title;
		uptr<sdl::texture> _txt_prompt;

		void refresh() final { load_textures(); }

		void load_textures();
	};
}
