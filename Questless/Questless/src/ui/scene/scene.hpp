//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "quantities/wall_time.hpp"

#include <deque>

namespace ql::scene {
	enum class update_result { continue_game, game_over };

	//! A scene in the game.
	class scene {
	public:
		scene();

		virtual ~scene() = default;

		//! Advances this scene by @p accrued_time.
		update_result update(sec& accrued_time, clock::time_point& last_update_time);

		//! Renders this scene to the display.
		void draw();
		
		//! The point in time at which the game switched to this scene.
		clock::time_point start_time() const { return _start_time; }
	private:
		clock::time_point _start_time;

		std::deque<per_sec> _fps_buffer;

		//! Specialized update logic for the scene subclass.
		virtual update_result subupdate() = 0;

		//! Specialized render logic for the scene subclass.
		virtual void subdraw() = 0;
	};
}
