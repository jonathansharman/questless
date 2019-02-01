//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "quantities/wall_time.hpp"

#include <SFML/Graphics.hpp>

#include <deque>

namespace ql::scene {
	class input_manager;

	enum class update_result { continue_game, game_over };

	//! A scene in the game.
	class scene : sf::Drawable {
	public:
		virtual ~scene() = default;

		//! Advances this scene according to the amount of time that has passed since the last update.
		//! @param im The input manager to use when updating this scene.
		update_result update(input_manager& im);

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

		//! The point in time at which the game switched to this scene.
		clock::time_point start_time() const {
			return _start_time;
		}

	private:
		clock::time_point _start_time = clock::now();
		clock::time_point _last_update_time = _start_time;
		sec _accrued_duration = 0.0_s;

		std::deque<per_sec> _fps_buffer;

		//! Specialized update logic for the scene subclass.
		virtual update_result scene_subupdate(sec elapsed_time, input_manager& im) = 0;

		//! Specialized render logic for the scene subclass.
		virtual void scene_subdraw() = 0;
	};
}
