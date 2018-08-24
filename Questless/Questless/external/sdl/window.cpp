//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "sdl/window.hpp"

#include "SDL_image.h"

using namespace units;

namespace sdl {
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

		refresh_position();
		refresh_size();
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

	window::~window() {
		SDL_DestroyWindow(_sdl_window);
	}

	void window::refresh_size() {
		SDL_GetWindowSize(_sdl_window, &_layout.size.x(), &_layout.size.y());
	}

	void window::refresh_position() {
		SDL_GetWindowPosition(_sdl_window, &_layout.position.x(), &_layout.position.y());
	}

	bool window::maximized() const {
		return (flags() & SDL_WINDOW_MAXIMIZED) != 0;
	}

	bool window::fullscreen() const {
		return (flags() & SDL_WINDOW_FULLSCREEN) != 0;
	}

	bool window::resizable() const {
		return (flags() & SDL_WINDOW_RESIZABLE) != 0;
	}

	window_space::point window::window_center() const {
		return window_space::point{width() / 2, height() / 2};
	}
	window_space::scalar window::window_center_x() const {
		return width() / 2;
	}
	window_space::scalar window::window_center_y() const {
		return height() / 2;
	}

	view_space::point window::view_center() const {
		return view_space::point{width() / 2.0f, height() / 2.0f};
	}
	view_space::scalar window::view_center_x() const {
		return width() / 2.0f;
	}
	view_space::scalar window::view_center_y() const {
		return height() / 2.0f;
	}
}
