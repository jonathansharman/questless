//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "bounded/static.hpp"
#include "quantities/wall_time.hpp"
#include "rsrc/fonts.hpp"
#include "utility/reference.hpp"

#include <SFML/Graphics.hpp>

#include <deque>

namespace ql {
	struct widget;

	//! Represents an instance of the game Questless.
	struct game {
		//! @param fullscreen Whether to run the game in fullscreen mode.
		game(bool fullscreen);

		~game();

		//! Runs a new game of Questless.
		void run();

	private:
		sf::RenderWindow _window;

		rsrc::fonts _fonts;

		uptr<widget> _root;

		// Timing

		//! The last time the game updated.
		clock::time_point _last_update_time = clock::now();

		//! Time debt is nonnegative.
		static constexpr sec min_time_debt = 0.0_s;
		//! Capped to prevent too much "fast-forwarding".
		static constexpr sec max_time_debt = 1.0_s;
		//! How far behind the target frame duration the scene is.
		static_bounded<sec, min_time_debt, max_time_debt> _time_debt = 0.0_s;

		//! The last however many instantaneous FPS counts, used to smooth out the FPS estimation.
		std::deque<per_sec> _fps_buffer;

		//! Tries to keep the scene running at the target frame rate.
		//! @return The duration of the last frame.
		auto regulate_timing() -> sec;

		//! Draws the FPS counter.
		auto draw_fps() -> void;
	};
}
