//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "input_manager.hpp"

namespace ql {
	using namespace spaces::window::literals;

	sf::Keyboard::Key input_manager::index_to_num_key(size_t index) {
		switch (index) {
			case 0: return sf::Keyboard::Key::Num1;
			case 1: return sf::Keyboard::Key::Num2;
			case 2: return sf::Keyboard::Key::Num3;
			case 3: return sf::Keyboard::Key::Num4;
			case 4: return sf::Keyboard::Key::Num5;
			case 5: return sf::Keyboard::Key::Num6;
			case 6: return sf::Keyboard::Key::Num7;
			case 7: return sf::Keyboard::Key::Num8;
			case 8: return sf::Keyboard::Key::Num9;
			case 9: return sf::Keyboard::Key::Num0;
			default: throw std::out_of_range{"Invalid number key index: " + std::to_string(index) + '.'};
		}
	}

	void input_manager::update() {
		_input_event_buffer.clear();
		_scroll = 0;
		_window_resized = false;

		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed:
					_quit = true;
					window.close();
					return;
				case sf::Event::KeyPressed:
					_input_event_buffer.push_back(input_event{event.key.code, true});
					break;
				case sf::Event::KeyReleased:
					_input_event_buffer.push_back(input_event{event.key.code, false});
					break;
				case sf::Event::MouseButtonPressed:
					_input_event_buffer.push_back(input_event{event.mouseButton.button, true});
					break;
				case sf::Event::MouseButtonReleased:
					_input_event_buffer.push_back(input_event{event.mouseButton.button, false});
					break;
				case sf::Event::MouseMoved:
					_mouse_movement = {event.mouseMove.x, event.mouseMove.y};
					break;
				case sf::Event::MouseWheelScrolled:
					_scroll += lround(event.mouseWheelScroll.delta);
					break;
				case sf::Event::Resized:
					_window_resized = true;
					break;
			}
		}
	}

	bool input_manager::up(input input) const {
		return !std::holds_alternative<sf::Keyboard::Key>(input)
			? sf::Keyboard::isKeyPressed(std::get<sf::Keyboard::Key>(input))
			: sf::Mouse::isButtonPressed(std::get<sf::Mouse::Button>(input));
	}

	bool input_manager::down(input input) const {
		return std::holds_alternative<sf::Keyboard::Key>(input)
			? sf::Keyboard::isKeyPressed(std::get<sf::Keyboard::Key>(input))
			: sf::Mouse::isButtonPressed(std::get<sf::Mouse::Button>(input));
	}

	bool input_manager::up(std::set<input> inputs) const {
		return std::any_of(inputs.begin(), inputs.end(), [this](input i) { return up(i); });
	}

	bool input_manager::down(std::set<input> inputs) const {
		return std::any_of(inputs.begin(), inputs.end(), [this](input i) { return down(i); });
	}

	bool input_manager::pressed(input input) const {
		return std::find(_input_event_buffer.begin(), _input_event_buffer.end(), input_event{input, true}) != _input_event_buffer.end();
	}

	bool input_manager::released(input input) const {
		return std::find(_input_event_buffer.begin(), _input_event_buffer.end(), input_event{input, false}) != _input_event_buffer.end();
	}

	bool input_manager::pressed(std::set<input> inputs) const {
		return std::find_if(_input_event_buffer.begin(), _input_event_buffer.end(),
			[&inputs](input_event ie) {
				return ie.pressed && inputs.find(ie.input) != inputs.end();
			}) != _input_event_buffer.end();
	}

	bool input_manager::released(std::set<input> inputs) const {
		return std::find_if(_input_event_buffer.begin(), _input_event_buffer.end(),
			[&inputs](input_event ie) {
				return !ie.pressed && inputs.find(ie.input) != inputs.end();
			}) != _input_event_buffer.end();
	}

	int input_manager::press_count(input input) const {
		return std::count(_input_event_buffer.begin(), _input_event_buffer.end(), input_event{input, true});
	}

	int input_manager::release_count(input input) const {
		return std::count(_input_event_buffer.begin(), _input_event_buffer.end(), input_event{input, false});
	}

	bool input_manager::press_count(std::set<input> inputs) const {
		return std::count_if(_input_event_buffer.begin(), _input_event_buffer.end(),
			[&inputs](input_event const& ie) {
				return !ie.pressed && inputs.find(ie.input) != inputs.end();
			});
	}

	bool input_manager::release_count(std::set<input> inputs) const {
		return std::count_if(_input_event_buffer.begin(), _input_event_buffer.end(),
			[&inputs](input_event const& ie) {
				return !ie.pressed && inputs.find(ie.input) != inputs.end();
			});
	}

	bool input_manager::any_pressed(bool include_mouse_buttons) const {
		return std::find_if(_input_event_buffer.begin(), _input_event_buffer.end(),
			[include_mouse_buttons](input_event ie) {
				return ie.pressed && (include_mouse_buttons || std::holds_alternative<sf::Keyboard::Key>(ie.input));
			}) != _input_event_buffer.end();
	}

	bool input_manager::any_released(bool include_mouse_buttons) const {
		return std::find_if(_input_event_buffer.begin(), _input_event_buffer.end(),
			[include_mouse_buttons](input_event ie) {
				return !ie.pressed && (include_mouse_buttons || std::holds_alternative<sf::Keyboard::Key>(ie.input));
			}) != _input_event_buffer.end();
	}

	int input_manager::any_press_count(bool include_mouse_buttons) const {
		return std::count_if(_input_event_buffer.begin(), _input_event_buffer.end(),
			[include_mouse_buttons](input_event ie) {
				return ie.pressed && (include_mouse_buttons || std::holds_alternative<sf::Keyboard::Key>(ie.input));
			});
	}

	int input_manager::any_release_count(bool include_mouse_buttons) const {
		return std::count_if(_input_event_buffer.begin(), _input_event_buffer.end(),
			[include_mouse_buttons](input_event ie) {
				return !ie.pressed && (include_mouse_buttons || std::holds_alternative<sf::Keyboard::Key>(ie.input));
			});
	}

	bool input_manager::shift() const {
		return sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)
			|| sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
	}

	bool input_manager::ctrl() const {
		return sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
			|| sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);
	}

	bool input_manager::alt() const {
		return sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)
			|| sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt);
	}

	void input_manager::move_mouse(sf::Vector2i const& position) {
		sf::Mouse::setPosition(position);
	}
}
