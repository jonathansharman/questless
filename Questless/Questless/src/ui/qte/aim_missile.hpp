//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "ui/dialog.hpp"
#include "world/world.hpp"

#include "media/spaces/view.hpp"

namespace ql {
	struct being;
	struct body_part;
}

namespace ql::qte {
	//! Quick time event for determining the .
	struct aim_missile : dialog {
		//! @param target_coords The target strike coordinates.
		//! @param cont The dialog continuation function.
		aim_missile(region_tile::point source_coords, being const& target_being, std::function<void(body_part*)> cont);

		void update(sec elapsed_time, input_manager& im) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

	private:
		enum class aiming_state { beginning, aiming, in_flight, ending };

		static constexpr auto _time_limit = 5.0_s;

		region_tile::point _source_tile_coords;
		being const& _target_being;
		media::spaces::view::point _target_view_coords;
		cancel::unitless<media::spaces::view::length::rep> _target_view_scale;
		continuation<body_part*> _cont;
		media::spaces::view::circle _aiming_circle;
		aiming_state _aiming_state = aiming_state::beginning;

		sec _total_elapsed_time = 0.0_s;

		sf::Text _title;
		sf::Text _prompt;
	};
}
