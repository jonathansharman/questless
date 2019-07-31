//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "scene.hpp"

#include "rsrc/fonts.hpp"
#include "utility/visitation.hpp"

#include "cancel/serialization.hpp"

#include <SFML/Graphics.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>

#include <numeric>
#include <thread>

namespace ql::scenes {
	scene::scene(rsrc::fonts const& fonts) : fonts{fonts} {}

	update_result scene::update(sf::Window& window) {
		// Regulate timing and get the elapsed time.
		auto const elapsed_time = regulate_timing();

		// Build event list.
		std::vector<sf::Event> events;
		sf::Event event;
		while (window.pollEvent(event)) {
			// End the game immediately if there was a quit event.
			switch (event.type) {
				case sf::Event::Closed:
					return game_over{};
				case sf::Event::KeyPressed:
					if (event.key.alt && event.key.code == sf::Keyboard::F4) { return game_over{}; }
					break;
				default:
					break;
			}
			// Otherwise, just add the event to the event list.
			events.push_back(event);
		}

		// Perform scene-specific updates.
		return scene_subupdate(elapsed_time, events);
	}

	void scene::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		// Clear the screen (or other render target).
		target.clear();

		// Draw scene-specific objects.
		scene_subdraw(target, states);

		// Draw the FPS counter.
		draw_fps(target, states);
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
			_time_debt += time_spent - target_frame_duration;
		}

		// Update FPS buffer.
		if (time_spent != 0.0_s) { _fps_buffer.push_back(1.0f / time_spent); }
		constexpr std::size_t max_fps_buffer_size = 25;
		if (_fps_buffer.size() > max_fps_buffer_size) { _fps_buffer.pop_front(); }

		return time_spent;
	}

	void scene::draw_fps(sf::RenderTarget& target, sf::RenderStates states) const {
		per_sec const fps_buffer_sum = std::reduce(_fps_buffer.begin(), _fps_buffer.end(), 0.0_hz, std::plus<per_sec>{});
		per_sec const fps_buffer_avg = fps_buffer_sum / _fps_buffer.size();
		sf::Text fps_text{fmt::format("{:.2f}", fps_buffer_avg), fonts.firamono, 20};
		fps_text.setFillColor(sf::Color::White);
		target.draw(fps_text, states);
	}
}
