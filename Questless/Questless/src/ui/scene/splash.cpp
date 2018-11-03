//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "splash.hpp"

#include "utility/random.hpp"

#include "sdl/resources.hpp"

using namespace sdl;
using namespace units;

namespace ql::scene {
	namespace {
		constexpr sec fade_out_duration = 2.0_s;
		constexpr sec fade_in_duration = 2.0_s;
		constexpr sec duration = fade_out_duration + fade_in_duration;
	}

	splash::splash()
		: _txt_splash_logo{"resources/textures/splash/logo.png"}
		, _txt_splash_flame{"resources/textures/splash/flame.png"}
		, _sfx_splash{"resources/sounds/splash.wav"}
	{}

	update_result splash::subupdate() {
		// Play the splash sound effect on the first frame of the splash screen.
		if (!_sound_played) {
			_sound_played = true;
			_sfx_splash.play();
		}

		static constexpr int flames_count = 20;
		static constexpr units::game_space::speed splash_flames_vy{-2800.0};

		// Move splash flames.
		for (window_space::point& position : _flame_positions) {
			position.y() += lround(_flames_vy * target_frame_duration);
			if (position.y() < 0) {
				position.y() += the_window().height() + _txt_splash_flame->height();
				position.x() = uniform(0, the_window().width() - 1);
			}
		}

		if (the_input().any_presses() || to_sec_quantity(clock::now() - start_time()) >= duration) {
			_sfx_splash.stop();
			_state = state::menu;
		}
		return update_result::continue_game;
	};

	void splash::subdraw() {
		float const intensity = [&] {
			if (to_sec_quantity(clock::now() - start_time()) < fade_in_duration) {
				auto ms_fading_in = std::chrono::duration_cast<game_space::seconds>(clock::now() - start_time()).count();
				return static_cast<float>(ms_fading_in / fade_in_duration.value);
			} else {
				auto ms_fading_out = std::chrono::duration_cast<game_space::seconds>(clock::now() - start_time() - fade_in_duration).count();
				return static_cast<float>(1.0 - ms_fading_out / fade_out_duration.value);
			}
		}();

		constexpr int logo_jiggle = 3;
		window_space::point logo_position = the_window().window_center() + random_displacement
			+ window_space::vector{uniform(-logo_jiggle, logo_jiggle), uniform(-logo_jiggle, logo_jiggle)};
		_txt_splash_logo.draw(logo_position, texture_space::align_center, texture_space::align_middle, colors::color_vector{intensity, intensity, intensity, 1.0f});

		for (window_space::point position : _splash_flame_positions) {
			_txt_splash_flame.draw(position, texture_space::align_center, texture_space::align_bottom, colors::color_vector{intensity, intensity, intensity, 1.0f});
		}
	}
}
