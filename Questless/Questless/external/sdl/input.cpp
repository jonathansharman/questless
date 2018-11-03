//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "input.hpp"

#include "resources.hpp"

#include <gsl/span>

using namespace units::literals;

namespace sdl {
	namespace {
		gsl::span<Uint8 const> const& keyboard_state() {
			static int key_count;
			static gsl::span<Uint8 const> result;
			static bool first_call = true;
			if (first_call) {
				auto temp = SDL_GetKeyboardState(&key_count);
				result = gsl::span<Uint8 const>{temp, key_count};
				first_call = false;
			}
			return result;
		}
	}

	decltype(SDLK_1) input::index_to_num_key(size_t index) {
		switch (index) {
			case 0: return SDLK_1;
			case 1: return SDLK_2;
			case 2: return SDLK_3;
			case 3: return SDLK_4;
			case 4: return SDLK_5;
			case 5: return SDLK_6;
			case 6: return SDLK_7;
			case 7: return SDLK_8;
			case 8: return SDLK_9;
			case 9: return SDLK_0;
			default: throw std::out_of_range{"Invalid number key index: " + std::to_string(index) + '.'};
		}
	}

	input::input()
		: _quit{false}
		, _window_resized{false}
		, _prev_keyboard_state(keyboard_state().size(), 0)
		, _curr_mouse_state{0}
		, _prev_mouse_state{0}
		, _mouse_position{0_px, 0_px}
		, _prev_mouse_position{0_px, 0_px}
	{}

	void input::update() {
		for (std::size_t i = 0; i < _prev_keyboard_state.size(); ++i) {
			_prev_keyboard_state[i] = keyboard_state()[i];
		}

		_quit = false;
		_window_resized = false;

		_prev_mouse_state = _curr_mouse_state;
		_prev_mouse_position = _mouse_position;
		_curr_mouse_state = SDL_GetMouseState(&_mouse_position.x().value, &_mouse_position.y().value);

		_scroll = 0;

		_press_buffer.clear();
		_release_buffer.clear();

		_presses.clear();
		_releases.clear();

		while (SDL_PollEvent(&_event)) {
			switch (_event.type) {
				case SDL_QUIT:
					_quit = true;
					return;
				case SDL_KEYDOWN:
					if (!_event.key.repeat) {
						_press_buffer.push_back(_event.key.keysym.sym);
						++_presses[_event.key.keysym.sym];
					}
					break;
				case SDL_KEYUP:
					if (!_event.key.repeat) {
						_release_buffer.push_back(_event.key.keysym.sym);
						++_releases[_event.key.keysym.sym];
					}
					break;
				case SDL_MOUSEWHEEL:
					_scroll += _event.wheel.y;
					break;
				case SDL_WINDOWEVENT:
					switch (_event.window.event) {
						case SDL_WINDOWEVENT_RESIZED: [[fallthrough]];
						case SDL_WINDOWEVENT_MAXIMIZED: [[fallthrough]];
						case SDL_WINDOWEVENT_RESTORED:
							the_window().refresh_size();
							the_window().refresh_position();
							_window_resized = true;
							break;
						case SDL_WINDOWEVENT_MOVED:
							the_window().refresh_position();
						default:
							break;
					}
					break;
			}
		}
	}

	int input::presses(SDL_Keycode key) const {
		return static_cast<int>(_presses.count(key));
	}

	int input::releases(SDL_Keycode key) const {
		return static_cast<int>(_releases.count(key));
	}

	bool input::up(SDL_Keycode key) const {
		return !keyboard_state()[SDL_GetScancodeFromKey(key)];
	}

	bool input::down(SDL_Keycode key) const {
		return keyboard_state()[SDL_GetScancodeFromKey(key)] == 1;
	}

	int input::any_presses(bool include_mouse_buttons) const {
		//! @todo This is returning a bool...

		if (include_mouse_buttons && _curr_mouse_state & (_curr_mouse_state ^ _prev_mouse_state)) {
			return true;
		}
		return !_press_buffer.empty();
	}

	int input::any_releases(bool include_mouse_buttons) const {
		//! @todo This is also returning a bool...

		if (include_mouse_buttons && ~_curr_mouse_state & (_curr_mouse_state ^ _prev_mouse_state)) {
			return true;
		}
		return !_release_buffer.empty();
	}

	bool input::any_down(bool include_mouse_buttons) const {
		if (include_mouse_buttons && _curr_mouse_state) {
			return true;
		}
		for (auto key_state : keyboard_state()) {
			if (key_state) {
				return true;
			}
		}
		return false;
	}

	bool input::any_up(bool include_mouse_buttons) const {
		if (include_mouse_buttons && !_curr_mouse_state) {
			return true;
		}
		for (auto key_state : keyboard_state()) {
			if (!key_state) {
				return true;
			}
		}
		return false;
	}

	bool input::shift() const {
		return keyboard_state()[SDL_SCANCODE_LSHIFT] || keyboard_state()[SDL_SCANCODE_RSHIFT];
	}

	bool input::ctrl() const {
		return keyboard_state()[SDL_SCANCODE_LCTRL] || keyboard_state()[SDL_SCANCODE_RCTRL];
	}

	bool input::alt() const {
		return keyboard_state()[SDL_SCANCODE_LALT] || keyboard_state()[SDL_SCANCODE_RALT];
	}

	bool input::meta() const {
		return keyboard_state()[SDL_SCANCODE_LGUI] || keyboard_state()[SDL_SCANCODE_RGUI];
	}

	bool input::pressed(mouse_button button) const {
		switch(button) {
			case mouse_button::left:
				return (_curr_mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) && !(_prev_mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT));
			case mouse_button::right:
				return (_curr_mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT)) && !(_prev_mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT));
			case mouse_button::middle:
				return (_curr_mouse_state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) && !(_prev_mouse_state & SDL_BUTTON(SDL_BUTTON_MIDDLE));
			default:
				return false;
		}
	}

	bool input::released(mouse_button button) const {
		switch(button) {
			case mouse_button::left:
				return !(_curr_mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) && (_prev_mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT));
			case mouse_button::right:
				return !(_curr_mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT)) && (_prev_mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT));
			case mouse_button::middle:
				return !(_curr_mouse_state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) && (_prev_mouse_state & SDL_BUTTON(SDL_BUTTON_MIDDLE));
			default:
				return false;
		}
	}

	bool input::down(mouse_button button) const {
		switch(button) {
			case mouse_button::left:
				return (_curr_mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
			case mouse_button::right:
				return (_curr_mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
			case mouse_button::middle:
				return (_curr_mouse_state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
			default:
				return false;
		}
	}

	bool input::up(mouse_button button) const {
		switch(button) {
			case mouse_button::left:
				return !(_curr_mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT));
			case mouse_button::right:
				return !(_curr_mouse_state & SDL_BUTTON(SDL_BUTTON_RIGHT));
			case mouse_button::middle:
				return !(_curr_mouse_state & SDL_BUTTON(SDL_BUTTON_MIDDLE));
			default:
				return false;
		}
	}

	void input::move_mouse(units::window_space::point const& position) {
		SDL_WarpMouseInWindow(the_window().sdl_ptr(), position.x().value, position.y().value);
		_mouse_position = position;
	}
}
