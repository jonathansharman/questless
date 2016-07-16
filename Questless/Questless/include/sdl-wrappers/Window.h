/**
* @file    Window.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for Window, a simple wrapper class around SDL_Window.
*/

#ifndef WINDOW_H
#define WINDOW_H

#include <string>
using std::string;
#include <stdexcept>
using std::runtime_error;
#include <memory>

#include <SDL.h>

#include "basic-sdl-wrappers.h"
#include "utility/optional.h"

namespace sdl
{
	class Window
	{
	public:
		using ptr = std::unique_ptr<Window>;

		/// Constructs a window object with the specified properties.
		/// @param title The title of the window.
		/// @param icon_filename The filename of the window's icon.
		/// @param fullscreen If true, window is created in fullscreen mode.
		/// @param width The width of the window in pixels.
		/// @param height The height of the window in pixels.
		/// @param h_centered If true, window is centered horizontally. Otherwise horizontal position is undefined.
		/// @param v_centered If true, window is centered vertically. Otherwise vertical position is undefined.
		/// @param resizable If true, window may be resized (enables maximize, restore, and window frame handles).
		Window(string title, string icon_filename, bool fullscreen = true, int width = 0, int height = 0, bool h_centered = false, bool v_centered = false, bool resizable = false);

		/// Constructs a window object with the specified properties.
		/// @param title The title of the window.
		/// @param icon_filename The filename of the window's icon.
		/// @param fullscreen If true, window is created in fullscreen mode.
		/// @param width The width of the window in pixels.
		/// @param height The height of the window in pixels.
		/// @param x The window's x-coordinate.
		/// @param y The window's y-coordinate.
		/// @param resizable If true, window may be resized (enables maximize, restore, and window frame handles).
		Window(string title, string icon_filename, bool fullscreen, int width, int height, int x, int y, bool resizable);

		Window(const Window& other) = delete;
		Window(Window&& other) = default;

		/// A convenience function for making a unique window pointer.
		template <typename... Args>
		static ptr make(Args&&... args) { return make_unique<Window>(std::forward<Args>(args)...); }

		~Window();

		Window& operator =(const Window& other) = delete;
		Window& operator =(Window&& other) = default;


		/// Creates a new window pointer with the same properties as the old one and destroys the old one.
		void recreate();

		/// @return The internal SDL_Window pointer.
		SDL_Window* const sdl_ptr() const { return _sdl_window; }

		/// @return The window title.
		string title() const { return _title; }

		/// @return The filename used to load the window icon.
		string icon_filename() const { return _icon_filename; }

		/// @return Whether the window is maximized.
		bool maximized() const;

		/// @return Whether the window is in fullscreen mode.
		bool fullscreen() const;

		/// @return Whether the window is in resizable mode.
		bool resizable() const;

		/// @return The position of the window.
		Point position() const;
		/// @return The x-coordinate of the window.
		int x() const;
		/// @return The y-coordinate of the window.
		int y() const;

		/// @return The resolution of the window, width x height.
		Point resolution() const;
		/// @return The window width.
		int width() const;
		/// @return The window height.
		int height() const;
		/// @return The center point of the window.
		Point center() const;
	private:
		SDL_Window* _sdl_window;
		string _title;
		string _icon_filename;

		mutable optional<int> _width, _height;

		/// @return The SDL window flags.
		Uint32 flags() const { return SDL_GetWindowFlags(_sdl_window); }
	};
}

#endif
