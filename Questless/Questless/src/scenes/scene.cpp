//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "scene.hpp"

#include "rsrc/fonts.hpp"
#include "ui/input_manager.hpp"
#include "utility/visitation.hpp"

#include <SFML/Graphics.hpp>
#include <fmt/format.h>

#include <numeric>
#include <thread>

namespace ql::scene {
	scene::scene(sf::Window& window, rsrc::fonts const& fonts) : _window{window}, _fonts{fonts} {}

	update_result scene::update(input_manager& im) {
		// Regulate timing and get the elapsed time.
		auto const elapsed_time = regulate_timing();

		// Update the input manager.
		im.update();

		// End the game if there was a quit event.
		if (im.quit() || im.alt() && im.pressed(sf::Keyboard::F4)) { return game_over{}; }

		// Perform scene-specific updates.
		return scene_subupdate(elapsed_time, im);
	}

	void scene::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		// Clear the screen (or other render target).
		target.clear();

		// Draw scene-specific objects.
		scene_subdraw(target, states);

		// Draw the FPS counter.
		draw_fps(target, states);

		// Display the scene to the window.
		_window.display();
	}

	sec scene::regulate_timing() {
		// Determine how much time has passed since the last call.
		sec const time_spent = to_sec(clock::now() - _last_update_time);
		_last_update_time = clock::now();

		if (time_spent < target_frame_duration) {
			// Under budget for this frame.
			if (_time_debt > 0.0_s) {
				//
			} else {
				// No time debt. Sleep for the difference.
				sec const sleep_duration = target_frame_duration - time_spent;
				std::this_thread::sleep_for(to_chrono_sec(sleep_duration));
			}
		} else {
			// Over budget. Add to time debt.
			_time_debt +=
		}

		// Increase time debt if too much time was spent on the last frame.
		_time_debt += std::max(0.0_s, time_spent - target_frame_duration);

		// Sleep if not enough time has passed.
		sec const sleep_duration = std::max(0.0_s, target_frame_duration - time_spent);
		std::this_thread::sleep_for(to_chrono_sec(sleep_duration));

		// Update FPS buffer.
		if (frame_duration != 0.0_s) { _fps_buffer.push_back(1.0 / frame_duration); }
		constexpr std::size_t max_fps_buffer_size = 25;
		if (_fps_buffer.size() > max_fps_buffer_size) { _fps_buffer.pop_front(); }

		return frame_duration;
	}

	void scene::draw_fps(sf::RenderTarget& target, sf::RenderStates states) const {
		auto const fps_buffer_sum = std::reduce(_fps_buffer.begin(), _fps_buffer.end(), 0.0_hz, std::plus<per_sec>{});
		auto const fps_buffer_avg = fps_buffer_sum / _fps_buffer.size();
		sf::Text fps_text{fmt::format("{:.2f}", fps_buffer_avg), _fonts.firamono, 20};
		fps_text.setColor(sf::Color::White);
		fps_text.setOrigin(fps_text.getLocalBounds().width, fps_text.getLocalBounds().height);
		fps_text.setPosition(sf::Vector2f{_window.getSize()});
		fps_text.draw(target, states);
	}

}
