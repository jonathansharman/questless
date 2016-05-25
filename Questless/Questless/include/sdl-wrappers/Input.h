/**
* @file    Input.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the input class. Handles mouse and keyboard input as well as other SDL events.
*/

#ifndef INPUT_H
#define INPUT_H

#include <vector>
using std::vector;
#include <map>
using std::map;
#include <algorithm>
#include <exception>
using std::bad_alloc;

#include <SDL.h>

#include "sdl-wrappers/basic-sdl-wrappers.h"
#include "sdl-wrappers/Window.h"

namespace sdl
{
	enum class MouseButton {left, right, middle};

	class Input
	{
	public:
		/// @return The number key corresponding to the given index, with the number keys ordered 1 to 9 and then 0.
		static decltype(SDLK_1) index_to_num_key(size_t index)
		{
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

		Input();
		Input(const Input& input);
		Input(Input&& input);

		~Input();

		Input& operator =(Input input);

		friend void swap(Input& first, Input& second);

		/// Updates the input state.
		/// @note Calls SDL_PumpEvents.
		void update();

		// Event accessors

		/// @return Whether the user has attempted to quit the application.
		bool quit() const { return _quit; }

		/// @return Whether the user has resized the window.
		bool window_resized() const { return _window_resized; }
		/// @return The window's new width after being resized. The value is unspecified if the window wasn't resized last update.
		int resized_window_width() const { return _resized_window_width; }
		/// @return The window's new height after being resized. The value is unspecified if the window wasn't resized last update.
		int resized_window_height() const { return _resized_window_height; }

		// Keyboard state accessors

		unsigned presses(SDL_Keycode key) const;
		unsigned releases(SDL_Keycode key) const;
		bool up(SDL_Keycode key) const;
		bool down(SDL_Keycode key) const;

		unsigned any_presses(bool include_mouse_buttons = true) const;
		unsigned any_releases(bool include_mouse_buttons = true) const;
		bool any_down(bool include_mouse_buttons = true) const;
		bool any_up(bool include_mouse_buttons = true) const;

		// Modifier key state accessors

		bool shift() const;
		bool ctrl() const;
		bool alt() const;
		bool meta() const;

		// Mouse state accessors
		
		bool mouse_visible() const { return SDL_ShowCursor(-1) == 1; }
		int x_mouse() const { return _x_mouse; }
		int y_mouse() const { return _y_mouse; }
		int last_x_mouse() const { return _prev_x_mouse; }
		int last_y_mouse() const { return _prev_y_mouse; }
		Point mouse_position() const { return Point(_x_mouse, _y_mouse); }
		Point last_mouse_position() const { return Point(_prev_x_mouse, _prev_y_mouse); }
		bool mouse_moved() const { return _x_mouse != _prev_x_mouse || _y_mouse != _prev_y_mouse; }

		bool pressed(MouseButton button) const;
		bool released(MouseButton button) const;
		bool down(MouseButton button) const;
		bool up(MouseButton button) const;

		/// @return The number of ticks the mouse wheel has been scrolled up since the last call to update. Negative value indicates scroll down.
		int scroll() const { return _scroll; }
		
		// Mouse state mutators

		/// Sets the position of the mouse cursor in the window.
		/// @param window The window relative to which to the cursor is moved.
		/// @param position The position to which the cursor is moved.
		void move_mouse(const Window& window, const Point& position);

		/// Hides the mouse cursor.
		void hide_mouse() const { SDL_ShowCursor(0); }

		/// Shows the mouse cursor.
		void show_mouse() const { SDL_ShowCursor(1); }
	private:
		SDL_Event _event;

		bool _quit;
		
		bool _window_resized = false;
		int _resized_window_width, _resized_window_height;

		int _key_count;
		uint8_t* _prev_keyboard_state;
		const uint8_t* _curr_keyboard_state;

		vector<SDL_Keycode> _press_buffer;
		vector<SDL_Keycode> _release_buffer;

		map<SDL_Keycode, unsigned> _presses;
		map<SDL_Keycode, unsigned> _releases;
		
		int _x_mouse;
		int _y_mouse;
		int _prev_x_mouse;
		int _prev_y_mouse;

		uint32_t _prev_mouse_state;
		uint32_t _curr_mouse_state;

		int _scroll; ///< The number of ticks the mouse wheel has been scrolled since the last call to update. Negative value indicates scroll down.
	};
}

#endif