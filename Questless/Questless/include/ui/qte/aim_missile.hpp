//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once


#include "animation/camera.hpp"
#include "ui/dialog.hpp"
#include "units/game_space.hpp"
#include "units/view_space.hpp"

namespace ql {
	class being;
	class body_part;
}

namespace ql::qte {
	//! Quick time event for determining the .
	class aim_missile : public dialog {
	public:
		//! @param target_coords The target strike coordinates.
		//! @param cont The dialog continuation function.
		aim_missile(region_tile::point source_coords, being const& target_being, std::function<void(body_part*)> cont);

		state update() final;

		void draw() const final;
	private:
		enum class aiming_state { beginning, aiming, in_flight, ending };

		static constexpr units::game_space::seconds _time_limit = 5.0s;

		region_tile::point _source_tile_coords;
		being const& _target_being;
		units::view_space::point _target_view_coords;
		units::view_space::scalar _target_view_scale;
		continuation<body_part*> _cont;
		units::view_space::sphere _aiming_circle;
		aiming_state _aiming_state = aiming_state::beginning;

		units::game_space::seconds _elapsed_time = 0.0s;

		uptr<sdl::texture> _txt_title;
		uptr<sdl::texture> _txt_prompt;
	};
}
