//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "sdl/input.hpp"
#include "sdl/resources.hpp"

namespace sdl
{
	input::input()
		: _quit{false}
		, _key_count{0}
		, _curr_mouse_state{0}
		, _prev_mouse_state{0}
		, _mouse_position{0, 0}
		, _prev_mouse_position{0, 0}
	{
		_curr_keyboard_state = SDL_GetKeyboardState(&_key_count);
		_prev_keyboard_state = new uint8_t[_key_count];
		for (int i = 0; i < _key_count; ++i) {
			_prev_keyboard_state[i] = 0;
		}
	}

	input::input(input const& the_input)
		: _quit{the_input._quit}
		, _key_count{the_input._key_count}
		, _curr_keyboard_state{the_input._curr_keyboard_state}
		, _prev_keyboard_state{new uint8_t[_key_count]}
		, _prev_mouse_state{the_input._prev_mouse_state}
		, _curr_mouse_state{the_input._curr_mouse_state}
		, _mouse_position(the_input._mouse_position)
		, _prev_mouse_position(the_input._prev_mouse_position)
	{
		for (int i = 0; i < _key_count; ++i) {
			_prev_keyboard_state[i] = the_input._prev_keyboard_state[i];
		}
	}

	input::input(input&& the_input)
	{
		swap(*this, the_input);
		the_input._prev_keyboard_state = nullptr;
	}

	input::~input()
	{
		delete [] _prev_keyboard_state;
	}

	input& input::operator =(input the_input) &
	{
		swap(*this, the_input);
		return *this;
	}

	void swap(input& first, input& second)
	{
		std::swap(first._quit, second._quit);
		std::swap(first._key_count, second._key_count);
		std::swap(first._curr_keyboard_state, second._curr_keyboard_state);
		std::swap(first._prev_keyboard_state, second._prev_keyboard_state);
		std::swap(first._prev_mouse_state, second._prev_mouse_state);
		std::swap(first._curr_mouse_state, second._curr_mouse_state);
		std::swap(first._mouse_position, second._mouse_position);
		std::swap(first._prev_mouse_position, second._prev_mouse_position);
	}

	void input::update()
	{
		for (int i = 0; i < _key_count; ++i) {
			_prev_keyboard_state[i] = _curr_keyboard_state[i];
		}

		_prev_mouse_state = _curr_mouse_state;
		_prev_mouse_position = _mouse_position;
		_curr_mouse_state = SDL_GetMouseState(&_mouse_position.x(), &_mouse_position.y());

		_scroll = 0;

		_press_buffer.clear();
		_release_buffer.clear();

		_presses.clear();
		_releases.clear();

		_window_maximized = false;
		_window_restored = false;
		_window_resized = false;

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
						case SDL_WINDOWEVENT_RESIZED:
							_window_resized = true;
							_resized_window_width = _event.window.data1;
							_resized_window_height = _event.window.data2;
							break;
						case SDL_WINDOWEVENT_MAXIMIZED:
							_window_maximized = true;
							break;
						case SDL_WINDOWEVENT_RESTORED:
							_window_restored = true;
							break;
						default:
							break;
					}
					break;
			}
		}
	}

	int input::presses(SDL_Keycode key) const
	{
		return static_cast<int>(_presses.count(key));
	}

	int input::releases(SDL_Keycode key) const
	{
		return static_cast<int>(_releases.count(key));
	}

	bool input::up(SDL_Keycode key) const
	{
		return !_curr_keyboard_state[SDL_GetScancodeFromKey(key)];
	}

	bool input::down(SDL_Keycode key) const
	{
		return _curr_keyboard_state[SDL_GetScancodeFromKey(key)] == 1;
	}

	int input::any_presses(bool include_mouse_buttons) const
	{
		//! @todo This is returning a bool...

		if (include_mouse_buttons && _curr_mouse_state & (_curr_mouse_state ^ _prev_mouse_state)) {
			return true;
		}
		return !_press_buffer.empty();
	}

	int input::any_releases(bool include_mouse_buttons) const
	{
		//! @todo This is also returning a bool...

		if (include_mouse_buttons && ~_curr_mouse_state & (_curr_mouse_state ^ _prev_mouse_state)) {
			return true;
		}
		return !_release_buffer.empty();
	}

	bool input::any_down(bool include_mouse_buttons) const
	{
		if (include_mouse_buttons && _curr_mouse_state) {
			return true;
		}
		for (int i = 0; i < _key_count; ++i) {
			if (_curr_keyboard_state[i]) {
				return true;
			}
		}
		return false;
	}

	bool input::any_up(bool include_mouse_buttons) const
	{
		if (include_mouse_buttons && !_curr_mouse_state) {
			return true;
		}
		for (int i = 0; i < _key_count; ++i) {
			if (!_curr_keyboard_state[i]) {
				return true;
			}
		}
		return false;
	}

	bool input::shift() const
	{
		return _curr_keyboard_state[SDL_SCANCODE_LSHIFT] || _curr_keyboard_state[SDL_SCANCODE_RSHIFT];
	}

	bool input::ctrl() const
	{
		return _curr_keyboard_state[SDL_SCANCODE_LCTRL] || _curr_keyboard_state[SDL_SCANCODE_RCTRL];
	}

	bool input::alt() const
	{
		return _curr_keyboard_state[SDL_SCANCODE_LALT] || _curr_keyboard_state[SDL_SCANCODE_RALT];
	}

	bool input::meta() const
	{
		return _curr_keyboard_state[SDL_SCANCODE_LGUI] || _curr_keyboard_state[SDL_SCANCODE_RGUI];
	}

	bool input::pressed(mouse_button button) const
	{
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

	bool input::released(mouse_button button) const
	{
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

	bool input::down(mouse_button button) const
	{
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

	bool input::up(mouse_button button) const
	{
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

	void input::move_mouse(units::screen_space::point const& position)
	{
		SDL_WarpMouseInWindow(the_window().sdl_ptr(), position.x(), position.y());
		_mouse_position = position;
	}
}
