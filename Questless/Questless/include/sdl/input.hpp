//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <stdexcept>
#include <vector>
#include <unordered_map>

#include <SDL.h>

#include "units/window_space.hpp"

namespace sdl
{
	enum class mouse_button { left, right, middle };

	//! Handles mouse and keyboard input as well as other SDL events.
	class input
	{
	public:
		//! The number key corresponding to the given index, with the number keys ordered 1 to 9 and then 0.
		static decltype(SDLK_1) index_to_num_key(size_t index);

		input();

		//! Updates the input state.
		//! @note Calls SDL_PumpEvents.
		void update();

		// Event accessors

		//! Whether the user has attempted to quit the application.
		bool quit() const { return _quit; }

		//! Whether the window resized during the last update.
		bool window_resized() const { return _window_resized; }

		// Keyboard state accessors

		int presses(SDL_Keycode key) const;
		int releases(SDL_Keycode key) const;
		bool up(SDL_Keycode key) const;
		bool down(SDL_Keycode key) const;

		int any_presses(bool include_mouse_buttons = true) const;
		int any_releases(bool include_mouse_buttons = true) const;
		bool any_down(bool include_mouse_buttons = true) const;
		bool any_up(bool include_mouse_buttons = true) const;

		// Modifier key state accessors

		bool shift() const;
		bool ctrl() const;
		bool alt() const;
		bool meta() const;

		// Mouse state accessors
		
		bool mouse_visible() const { return SDL_ShowCursor(-1) == 1; }
		auto x_mouse() const { return _mouse_position.x(); }
		auto y_mouse() const { return _mouse_position.y(); }
		auto last_x_mouse() const { return _prev_mouse_position.x(); }
		auto last_y_mouse() const { return _prev_mouse_position.y(); }
		units::window_space::point mouse_position() const { return _mouse_position; }
		units::window_space::point last_mouse_position() const { return _prev_mouse_position; }
		bool mouse_moved() const { return _mouse_position != _prev_mouse_position; }

		bool pressed(mouse_button button) const;
		bool released(mouse_button button) const;
		bool down(mouse_button button) const;
		bool up(mouse_button button) const;

		//! The number of ticks the mouse wheel has been scrolled up since the last call to update. Negative value indicates scroll down.
		int scroll() const { return _scroll; }
		
		// Mouse state mutators

		//! Sets the position of the mouse cursor in the window.
		//! @param position The position to which the cursor is moved.
		void move_mouse(units::window_space::point const& position);

		//! Hides the mouse cursor.
		void hide_mouse() const { SDL_ShowCursor(0); }

		//! Shows the mouse cursor.
		void show_mouse() const { SDL_ShowCursor(1); }
	private:
		SDL_Event _event;

		bool _quit;
		bool _window_resized;

		std::vector<uint8_t> _prev_keyboard_state;

		std::vector<SDL_Keycode> _press_buffer;
		std::vector<SDL_Keycode> _release_buffer;

		std::unordered_map<SDL_Keycode, int> _presses;
		std::unordered_map<SDL_Keycode, int> _releases;

		units::window_space::point _mouse_position;
		units::window_space::point _prev_mouse_position;

		uint32_t _prev_mouse_state;
		uint32_t _curr_mouse_state;

		int _scroll; //!< The number of ticks the mouse wheel has been scrolled since the last call to update. Negative value indicates scroll down.
	};
}
