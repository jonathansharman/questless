//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "quantities/wall_time.hpp"
#include "utility/reference.hpp"
#include "utility/static_bounded.hpp"

#include <SFML/Graphics.hpp>

#include <deque>
#include <variant>

namespace ql {
	namespace rsrc {
		struct fonts;
	}
	struct input_manager;

	namespace scene {
		struct continue_scene {};
		struct switch_scene {
			uptr<scene> new_scene;
		};
		struct game_over {};

		using update_result = std::variant<continue_scene, switch_scene, game_over>;

		//! A scene in the game.
		struct scene : public sf::Drawable {
			//! @param window The window in which this scene will be drawn.
			scene(sf::Window& window, rsrc::fonts const& fonts);

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
			sf::Window& _window;
			rsrc::fonts const& _fonts;

			//! When the scene started.
			clock::time_point _start_time = clock::now();
			//! The last time the scene was updated.
			clock::time_point _last_update_time = _start_time;

			//! Time debt is nonnegative.
			static constexpr sec min_time_debt = 0.0_s;
			//! Capped to prevent too much "fast-forwarding".
			static constexpr sec max_time_debt = 1.0_s;
			//! How far behind the target frame duration the scene is.
			static_bounded<sec, min_time_debt, max_time_debt> _time_debt = 0.0_s;

			//! The last however many instantaneous FPS counts, used to smooth out the FPS estimation.
			std::deque<per_sec> _fps_buffer;

			//! Specialized update logic for the scene subclass.
			virtual update_result scene_subupdate(sec elapsed_time, input_manager& im) = 0;

			//! Specialized render logic for the scene subclass.
			virtual void scene_subdraw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

			//! Tries to keep the scene running at the target frame rate.
			//! @return The duration of the last frame.
			sec regulate_timing();

			//! Draws the FPS counter.
			void draw_fps(sf::RenderTarget& target, sf::RenderStates states) const;
		};
	}
}
