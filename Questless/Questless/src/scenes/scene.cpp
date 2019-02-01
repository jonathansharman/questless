//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "scene.hpp"

#include <fmt/format.h>

#include <numeric>
#include <thread>

namespace ql::scene {
	update_result scene::update(input_manager& im) {
		im.update();

		if (im.quit() || im.alt() && im.pressed(sf::Keyboard::F4)) { return update_result::game_over; }

		// Update time.
		auto frame_duration = to_sec(clock::now() - _last_update_time);
		_last_update_time = clock::now();
		_accrued_duration += frame_duration;

		// Perform subtype-specific scene updates.
		if (scene_subupdate(frame_duration, im) == update_result::game_over) { return update_result::game_over; }

		// Regulate FPS.
		std::this_thread::sleep_for(to_chrono_sec(frame_duration - _accrued_duration));
		_accrued_duration -= frame_duration;
		// Cap accrued time.
		_accrued_duration = std::min(_accrued_duration, 1.0_s);

		// Update FPS counter.
		if (frame_duration != 0.0_s) { _fps_buffer.push_back(1.0 / frame_duration); }
		constexpr std::size_t max_fps_buffer_size = 25;
		if (_fps_buffer.size() > max_fps_buffer_size) { _fps_buffer.pop_front(); }

		return update_result::continue_game;
	}

	void scene::draw() {
		the_window().clear();

		scene_subdraw();

		// Draw FPS counter.
		auto const fps_buffer_sum = std::reduce(_fps_buffer.begin(), _fps_buffer.end(), 0.0_hz, std::plus<per_sec>{});
		auto const fps_buffer_avg = fps_buffer_sum / _fps_buffer.size();
		fmt::format("{:.2f}", fps_buffer_avg);
		texture txt_fps = _fnt_20pt->render(oss_fps.str().c_str(), colors::white());
		txt_fps.draw(window_space::point(the_window().width() - 1, the_window().height() - 1),
			texture_space::align_right,
			texture_space::align_bottom);

		the_window().display();
	}
}
