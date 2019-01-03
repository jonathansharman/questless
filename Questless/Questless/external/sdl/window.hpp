//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "spaces/window.hpp"

#include <SDL.h>

#include <string>
#include <memory>

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
		spaces::window::point position() const { return _layout.position; }
		//! The x-coordinate of the window in screen space.
		spaces::window::px x() const { return spaces::window::x(_layout.position); }
		//! The y-coordinate of the window in screen space.
		spaces::window::px y() const { return spaces::window::y(_layout.position); }

		//! The size/resolution of the window in screen space.
		spaces::window::vector size() const { return _layout.size; }
		//! The width of the window in screen space.
		spaces::window::px width() const { return spaces::window::x(_layout.size); }
		//! The height of the window in screen space.
		spaces::window::px height() const { return spaces::window::y(_layout.size); }

		//! The center point of the window, in window space.
		spaces::window::point window_center() const;
		//! The horizontal center of the window, in window space.
		spaces::window::px window_center_x() const;
		//! The vertical center of the window, in window space.
		spaces::window::px window_center_y() const;

		//! The center point of the window, in view space.
		spaces::view::point view_center() const;
		//! The horizontal center of the window, in view space.
		spaces::view::length view_center_x() const;
		//! The vertical center of the window, in view space.
		spaces::view::length view_center_y() const;
	private:
		SDL_Window* _sdl_window;
		std::string _title;
		std::string _icon_filename;

		spaces::window::box _layout;

		//! The SDL window flags.
		Uint32 flags() const { return SDL_GetWindowFlags(_sdl_window); }
	};
}
