//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "game.hpp"

#include "ui/splash.hpp"
#include "ui/widget.hpp"

#include "cancel/serialization.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>

#include <numeric>
#include <thread>

namespace ql {
	game::game(bool fullscreen) {
#if true
		constexpr int _dflt_window_width = 1024;
		constexpr int _dflt_window_height = 768;
#else
		constexpr int _dflt_window_width = 1920;
		constexpr int _dflt_window_height = 1080;
#endif
		auto const window_style = fullscreen ? sf::Style::Fullscreen : sf::Style::Default;
		_window.create(sf::VideoMode{_dflt_window_width, _dflt_window_height}, "Questless", window_style);
		auto icon = rsrc::load<sf::Image>("resources/textures/icon.png");
		_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

		// Start on the splash screen.
		_root = umake<splash>(_root, _fonts);

		// Communicate the initial window size, and set position.
		_root->on_parent_resize(view::vector_from_sfml(_window.getSize()));
		_root->set_position({});
	}

	game::~game() = default;

	void game::run() {
		for (;;) {
			// Handle events.
			sf::Event event;
			while (_window.pollEvent(event)) {
				// Shove the event_handled object into an unused variable because the root is the only handler.
				[[maybe_unused]] auto handled = widget::event_handled::yes;
				switch (event.type) {
					case sf::Event::Closed:
						// End immediately if the window is closed.
						return;
					case sf::Event::Resized: {
						// Rescale view to fit the new window size.
						auto const float_width = static_cast<float>(event.size.width);
						auto const float_height = static_cast<float>(event.size.height);
						_window.setView(sf::View{sf::FloatRect{0.0f, 0.0f, float_width, float_height}});
						// Inform the root widget.
						_root->on_parent_resize(view::vector_from_size_event(event.size));
						break;
					}
					case sf::Event::KeyPressed:
						if (event.key.alt && event.key.code == sf::Keyboard::F4) {
							// End immediately on Alt-F4.
							return;
						} else {
							handled = _root->on_key_press(event.key);
						}
						break;
					case sf::Event::KeyReleased:
						handled = _root->on_key_release(event.key);
						break;
					case sf::Event::TextEntered:
						handled = _root->on_text_entry(event.text.unicode);
						break;
					case sf::Event::MouseButtonPressed:
						handled = _root->on_mouse_press(event.mouseButton);
						break;
					case sf::Event::MouseButtonReleased:
						handled = _root->on_mouse_release(event.mouseButton);
						break;
					case sf::Event::MouseMoved:
						_root->on_mouse_move(view::point_from_mouse_move_event(event.mouseMove));
						break;
					case sf::Event::MouseWheelScrolled:
						handled = _root->on_mouse_wheel_scroll(event.mouseWheelScroll);
						break;
					default:
						break;
				}
			}
			// Check if the game ended via an event.
			if (_root == nullptr) { return; }

			// Update.
			_root->update(regulate_timing());
			// Check if the game ended via the update.
			if (_root == nullptr) { return; }

			// Draw.
			_window.clear();
			_window.draw(*_root);
			draw_fps();
			_window.display();
		}
	}

	auto game::regulate_timing() -> sec {
		// Determine how much time has passed since the last call.
		sec const time_spent = to_sec(clock::now() - _last_update_time);
		_last_update_time = clock::now();

		auto time_deficit = time_spent - target_frame_duration;
		if (time_deficit > 0.0_s) {
			// Over budget. Add to time debt.
			_time_debt += time_deficit;
		} else {
			// Under budget. Pay some time debt and/or wait.
			sec time_surplus = -time_deficit;
			if (_time_debt < time_surplus) {
				// Pay full debt.
				time_surplus -= _time_debt.value();
				_time_debt = 0.0_s;
				// Sleep for remaining time surplus.
				std::this_thread::sleep_for(to_chrono_sec(time_surplus));
			} else {
				// Still behind but can pay part of the time debt.
				_time_debt -= time_surplus;
			}
		}

		// Update FPS buffer.
		if (time_spent != 0.0_s) { _fps_buffer.push_back(1.0f / time_spent); }
		constexpr std::size_t max_fps_buffer_size = 25;
		if (_fps_buffer.size() > max_fps_buffer_size) { _fps_buffer.pop_front(); }

		return time_spent;
	}

	void game::draw_fps() {
		per_sec const fps_buffer_sum = std::reduce(_fps_buffer.begin(), _fps_buffer.end(), 0.0_hz, std::plus<per_sec>{});
		per_sec const fps_buffer_avg = fps_buffer_sum / _fps_buffer.size();
		sf::Text fps_text{fmt::format("{}", fps_buffer_avg), _fonts.firamono, 20};
		fps_text.setFillColor(sf::Color::White);
		_window.draw(fps_text);
	}
}
