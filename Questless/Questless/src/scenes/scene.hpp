//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "bounded/static.hpp"
#include "quantities/wall_time.hpp"
#include "utility/reference.hpp"

#include <SFML/Graphics.hpp>

#include <deque>
#include <variant>

namespace ql {
	namespace rsrc {
		struct fonts;
	}

	namespace scenes {
		struct scene;

		struct continue_scene {};
		struct switch_scene {
			uptr<scene> new_scene;
		};
		struct game_over {};

		using update_result = std::variant<continue_scene, switch_scene, game_over>;

		//! A scene in the game.
		struct scene : public sf::Drawable {
			rsrc::fonts const& fonts;

			scene(rsrc::fonts const& fonts);

			virtual ~scene() = default;

			//! Advances this scene by one frame.
			//! @param window The window containing this scene.
			update_result update(sf::Window& window);

			void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

			//! The point in time at which the game switched to this scene.
			clock::time_point start_time() const {
				return _start_time;
			}

			//! The length of time the game has been in this scene.
			sec get_time_in_state() const {
				return to_sec(clock::now() - _start_time);
			}

		private:
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
			virtual update_result scene_subupdate(sec elapsed_time, std::vector<sf::Event>& events) = 0;

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
