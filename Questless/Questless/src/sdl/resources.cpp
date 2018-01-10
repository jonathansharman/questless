//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "sdl/resources.hpp"
#include "sdl/renderable.hpp"

namespace sdl
{
	namespace
	{
		SDL_GLContext _opengl_context;
		std::unique_ptr<shader_program> _dflt_program;
		std::unique_ptr<shader_program> _solid_program;
		std::unique_ptr<renderer> _renderer = nullptr;
		std::unique_ptr<window> _window = nullptr;
	}

	SDL_GLContext& opengl_context() { return _opengl_context; }

	shader_program& dflt_program() { return *_dflt_program; }
	void set_dflt_program(std::unique_ptr<shader_program> shader_program) { _dflt_program = std::move(shader_program); }

	shader_program& solid_program() { return *_solid_program; }
	void set_solid_program(std::unique_ptr<shader_program> shader_program) { _solid_program = std::move(shader_program); }

	renderer& the_renderer() { return *_renderer; }
	void set_the_renderer(std::unique_ptr<renderer> renderer)
	{
		bool initializing = _renderer == nullptr;
		_renderer = std::move(renderer);
		if (!initializing && _renderer != nullptr) {
			// Reassigning the renderer. Need to invalidate shared textures and refresh all renderable objects.
			the_texture_manager().clear_cache();
			renderable::refresh_all();
		}
	}

	window& the_window() { return *_window; }
	void set_the_window(std::unique_ptr<window> the_window)
	{
		_window = std::move(the_window);
	}

	input& the_input() 
	{
		static input _input;
		return _input;
	}

	resource_manager<texture>& the_texture_manager()
	{
		static resource_manager<texture> _texture_manager;
		return _texture_manager;
	}

	resource_manager<font>& the_font_manager()
	{
		static resource_manager<font> _font_manager;
		return _font_manager;
	}

	resource_manager<sound>& the_sound_manager()
	{
		static resource_manager<sound> _sound_manager;
		return _sound_manager;
	}
}
