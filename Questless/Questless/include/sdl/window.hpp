//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <memory>
#include <optional>

#include <SDL.h>

#include "units/screen_space.hpp"

namespace sdl
{
	//! A simple wrapper around SDL_Window.
	class window
	{
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

		//! Creates a new window pointer with the same properties as the old one and destroys the old one.
		void recreate();

		//! The internal SDL_Window pointer.
		SDL_Window* const sdl_ptr() const { return _sdl_window; }

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

		//! The position of the window.
		units::screen_space::point position() const;
		//! The x-coordinate of the window.
		int x() const;
		//! The y-coordinate of the window.
		int y() const;

		//! The resolution of the window: width by height.
		units::screen_space::vector resolution() const;
		//! The window width.
		int width() const;
		//! The window height.
		int height() const;
		//! The center point of the window.
		units::screen_space::point center() const;
		//! The horizontal center of the window.
		int x_center() const;
		//! The vertical center of the window.
		int y_center() const;
	private:
		SDL_Window* _sdl_window;
		std::string _title;
		std::string _icon_filename;

		mutable std::optional<int> _width;
		mutable std::optional<int> _height;

		//! The SDL window flags.
		Uint32 flags() const { return SDL_GetWindowFlags(_sdl_window); }
	};
}