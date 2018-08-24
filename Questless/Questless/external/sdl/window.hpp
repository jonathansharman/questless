//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <memory>

#include <SDL.h>

#include "units/screen_space.hpp"
#include "units/window_space.hpp"

namespace sdl {
	//! A simple wrapper around SDL_Window.
	class window {
	public:
		//! Constructs a window object with the specified properties.
		//! @param title The title of the window.
		//! @param icon_filename The filename of the window's icon.
		//! @param fullscreen If true, window is created in fullscreen mode.
		//! @param width The width of the window in pixels.
		//! @param height The height of the window in pixels.
		//! @param h_centered If true, window is centered horizontally. Otherwise horizontal position is undefined.
		//! @param v_centered If true, window is centered vertically. Otherwise vertical position is undefined.
		//! @param resizable If true, window may be resized (enables maximize, restore, and window frame handles).
		//! @param grab_input If true, window captures input, meaning the mouse cannot leave the window.
		window
			( std::string title
			, std::string icon_filename
			, bool fullscreen = true
			, int width = 0
			, int height = 0
			, bool h_centered = false
			, bool v_centered = false
			, bool resizable = false
			, bool grab_input = false
			);

		//! Constructs a window object with the specified properties.
		//! @param title The title of the window.
		//! @param icon_filename The filename of the window's icon.
		//! @param fullscreen If true, window is created in fullscreen mode.
		//! @param width The width of the window in pixels.
		//! @param height The height of the window in pixels.
		//! @param x The window's x-coordinate.
		//! @param y The window's y-coordinate.
		//! @param resizable If true, window may be resized (enables maximize, restore, and window frame handles).
		//! @param grab_input If true, window captures input, meaning the mouse cannot leave the window.
		window(std::string title, std::string icon_filename, bool fullscreen, int width, int height, int x, int y, bool resizable, bool grab_input);

		window(window const&) = delete;
		window(window&&) = default;

		~window();

		window& operator =(window const&) = delete;
		window& operator =(window&&) & = default;

		//! Refreshes cached window size.
		void refresh_size();

		//! Refreshes cached window position.
		void refresh_position();

		//! The internal SDL_Window pointer.
		SDL_Window* sdl_ptr() const { return _sdl_window; }

		//! The window title.
		std::string title() const { return _title; }

		//! The filename used to load the window icon.
		std::string icon_filename() const { return _icon_filename; }

		//! Whether the window is maximized.
		bool maximized() const;

		//! Whether the window is in fullscreen mode.
		bool fullscreen() const;

		//! Whether the window is in resizable mode.
		bool resizable() const;

		//! The position of the window in screen space.
		units::screen_space::point position() const { return _layout.position; }
		//! The x-coordinate of the window in screen space.
		units::screen_space::scalar x() const { return _layout.position.x(); }
		//! The y-coordinate of the window in screen space.
		units::screen_space::scalar y() const { return _layout.position.y(); }

		//! The size/resolution of the window in screen space.
		units::screen_space::vector size() const { return _layout.size; }
		//! The width of the window in screen space.
		units::screen_space::scalar width() const { return _layout.size.x(); }
		//! The height of the window in screen space.
		units::screen_space::scalar height() const { return _layout.size.y(); }

		//! The center point of the window, in window space.
		units::window_space::point window_center() const;
		//! The horizontal center of the window, in window space.
		units::window_space::scalar window_center_x() const;
		//! The vertical center of the window, in window space.
		units::window_space::scalar window_center_y() const;

		//! The center point of the window, in view space.
		units::view_space::point view_center() const;
		//! The horizontal center of the window, in view space.
		units::view_space::scalar view_center_x() const;
		//! The vertical center of the window, in view space.
		units::view_space::scalar view_center_y() const;
	private:
		SDL_Window* _sdl_window;
		std::string _title;
		std::string _icon_filename;

		units::screen_space::box _layout;

		//! The SDL window flags.
		Uint32 flags() const { return SDL_GetWindowFlags(_sdl_window); }
	};
}
