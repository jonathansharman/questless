//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "sdl/window.hpp"

#include "SDL_image.h"

using namespace units;

namespace sdl
{
	window::window
		( std::string title
		, std::string icon_filename
		, bool fullscreen
		, int width
		, int height
		, bool h_centered
		, bool v_centered
		, bool resizable
		, bool grab_input
		)
		: _title{title}, _icon_filename{icon_filename}
	{
		_sdl_window = SDL_CreateWindow
			( title.c_str()
			, h_centered ? SDL_WINDOWPOS_CENTERED : SDL_WINDOWPOS_UNDEFINED
			, v_centered ? SDL_WINDOWPOS_CENTERED : SDL_WINDOWPOS_UNDEFINED
			, width, height
			, SDL_WINDOW_OPENGL | (fullscreen ? SDL_WINDOW_FULLSCREEN : 0) | (resizable ? SDL_WINDOW_RESIZABLE : 0) | (grab_input ? SDL_WINDOW_INPUT_GRABBED : 0)
			);
		if (_sdl_window == nullptr) {
			throw std::runtime_error{"Failed to create window."};
		}
		SDL_SetWindowIcon(_sdl_window, IMG_Load(_icon_filename.c_str()));
	}

	window::window
		( std::string title
		, std::string icon_filename
		, bool fullscreen
		, int width
		, int height
		, int x
		, int y
		, bool resizable
		, bool grab_input
		)
		: _title{title}, _icon_filename{icon_filename}
	{
		_sdl_window = SDL_CreateWindow
			( title.c_str()
			, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED
			, width, height
			, (fullscreen ? SDL_WINDOW_FULLSCREEN : 0) | (resizable ? SDL_WINDOW_RESIZABLE : 0) | (grab_input ? SDL_WINDOW_INPUT_GRABBED : 0)
			);
		if (_sdl_window == nullptr) {
			throw std::runtime_error{"Failed to create window."};
		}
		SDL_SetWindowIcon(_sdl_window, IMG_Load("textures/icon.png"));
		SDL_SetWindowPosition(_sdl_window, x, y);
	}

	window::~window()
	{
		SDL_DestroyWindow(_sdl_window);
	}

	void window::recreate()
	{
		// Cached width and height no longer valid.
		_width = std::nullopt;
		_height = std::nullopt;

		SDL_Window* old_sdl_window = _sdl_window;
		int x_init = x();
		int y_init = y();
		_sdl_window = SDL_CreateWindow
			( _title.c_str()
			, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED
			, width(), height()
			, flags()
			);
		SDL_SetWindowIcon(_sdl_window, IMG_Load(_icon_filename.c_str()));
		SDL_SetWindowPosition(_sdl_window, x_init, y_init);
		SDL_DestroyWindow(old_sdl_window);
	}

	bool window::maximized() const
	{
		return (flags() & SDL_WINDOW_MAXIMIZED) != 0;
	}

	bool window::fullscreen() const
	{
		return (flags() & SDL_WINDOW_FULLSCREEN) != 0;
	}

	bool window::resizable() const
	{
		return (flags() & SDL_WINDOW_RESIZABLE) != 0;
	}

	screen_space::point window::position() const
	{
		screen_space::point result;
		SDL_GetWindowPosition(_sdl_window, &result.x(), &result.y());
		return result;
	}
	int window::x() const
	{
		int x;
		SDL_GetWindowPosition(_sdl_window, &x, nullptr);
		return x;
	}
	int window::y() const
	{
		int y;
		SDL_GetWindowPosition(_sdl_window, nullptr, &y);
		return y;
	}

	screen_space::vector window::resolution() const
	{
		return screen_space::vector{width(), height()};
	}
	int window::width() const
	{
		if (_width) {
			return *_width;
		} else {
			int width;
			SDL_GetWindowSize(_sdl_window, &width, nullptr);
			_width = width;
			return width;
		}
	}
	int window::height() const
	{
		if (_height) {
			return *_height;
		} else {
			int height;
			SDL_GetWindowSize(_sdl_window, nullptr, &height);
			_height = height;
			return height;
		}
	}
	screen_space::point window::center() const
	{
		return screen_space::point{width() / 2, height() / 2};
	}
	screen_space::scalar window::x_center() const
	{
		return width() / 2;
	}
	screen_space::scalar window::y_center() const
	{
		return height() / 2;
	}
}
