//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

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
		constexpr int _dflt_window_width = 1024;
		constexpr int _dflt_window_height = 768;

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

	auto game::run() -> void {
		for (;;) {
			// Handle events.
			sf::Event event;
			while (_window.pollEvent(event)) {
				// Shove the event_handled object into an unused variable because the root is the only handler.
				[[maybe_unused]] auto handled = widget::event_handled::yes;
				switch (event.type) {
					case sf::Event::Closed:
						request_quit();
						break;
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
							// Alt-F4 to quit.
							request_quit();
							break;
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
			sec const elapsed_time = regulate_timing();
			_root->update(elapsed_time);
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
		// Determine how much time has actually passed since the last call.
		sec frame_duration = to_sec(clock::now() - _last_update_time);
		_last_update_time = clock::now();

		auto time_deficit = frame_duration - target_frame_duration;
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
				auto const sleep = std::chrono::duration_cast<std::chrono::milliseconds>(to_chrono_sec(time_surplus));
				std::this_thread::sleep_for(sleep);
				// Adjust frame duration to account for time slept.
				frame_duration += time_surplus;
			} else {
				// Still behind but can pay part of the time debt.
				_time_debt -= time_surplus;
			}
		}

		// Update FPS buffer.
		_avg_fps.push(1.0f / frame_duration);

		return frame_duration;
	}

	auto game::draw_fps() -> void {
		sf::Text fps_text{fmt::format("{}", _avg_fps.get()), _fonts.firamono, 20};
		fps_text.setOutlineColor(sf::Color::Black);
		fps_text.setOutlineThickness(1.0f);
		fps_text.setFillColor(sf::Color::White);
		_window.draw(fps_text);
	}

	auto game::request_quit() -> void {
		[[maybe_unused]] auto handled = _root->on_request_quit();
		// Root element must handle quit events.
		assert(handled == widget::event_handled::yes);
	}
}
