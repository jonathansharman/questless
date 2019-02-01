//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "splash.hpp"

#include "utility/random.hpp"

#include <algorithm>

using namespace media;

namespace ql::scene {
	namespace {
		constexpr sec fade_out_duration = 2.0_s;
		constexpr sec fade_in_duration = 2.0_s;
		constexpr sec duration = fade_out_duration + fade_in_duration;
	}

	splash::splash() {
		static constexpr int flames_count = 20;
		for (int i = 0; i < flames_count; ++i) {
			_flame_positions.push_back(
			    uniform(0, the_window().width() - 1), (i + 1) * the_window().height() / flame_count);
		}
	}

	update_result splash::scene_subupdate(sec elapsed_time, input_manager& im) {
		// Play the splash sound effect on the first frame of the splash screen.
		if (!_sound_played) {
			_sound_played = true;
			_loader.flame_sound.play();
		}

		static constexpr world::speed splash_flames_vy{-2800.0};

		// Move splash flames.
		for (window::point& position : _flame_positions) {
			position.y() += lround(splash_flames_vy * target_frame_duration);
			if (position.y() < 0) {
				position.y() += the_window().height() + _txt_splash_flame->height();
				position.x() = uniform(0, the_window().width() - 1);
			}
		}

		if (im.any_press_count() || to_sec(clock::now() - start_time()) >= duration) {
			_loader.flame_sound.stop();
			_state = state::menu;
		}
		return update_result::continue_game;
	};

	void splash::scene_subdraw() {
		float const intensity = [&] {
			if (to_sec(clock::now() - start_time()) < fade_in_duration) {
				auto time_fading_in = std::chrono::duration_cast<chrono_sec>(clock::now() - start_time());
				return time_fading_in / fade_in_duration;
			} else {
				auto time_fading_out = (clock::now() - start_time() - fade_in_duration);
				return 1.0 - time_fading_out / fade_out_duration;
			}
		}();

		constexpr int logo_jiggle = 3;
		window::point logo_position = the_window().window_center() + random_displacement
			+ window_space::vector{uniform(-logo_jiggle, logo_jiggle), uniform(-logo_jiggle, logo_jiggle)};
		_txt_splash_logo.draw(logo_position, texture_space::align_center, texture_space::align_middle, colors::color_vector{intensity, intensity, intensity, 1.0f});

		for (window_space::point position : _splash_flame_positions) {
			_txt_splash_flame.draw(position, texture_space::align_center, texture_space::align_bottom, colors::color_vector{intensity, intensity, intensity, 1.0f});
		}
	}
}
