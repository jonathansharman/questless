//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "sdl/resources.h"
#include "sdl/Renderable.h"

namespace sdl
{
	namespace
	{
		GLuint opengl_program = 0;
		std::unique_ptr<Renderer> _renderer = nullptr;
		std::unique_ptr<Window> _window = nullptr;
	}

	GLuint& dflt_program() { return opengl_program; }

	GLint vs_attr_position()
	{
		static bool first_call = true;
		static GLint result = -1;
		if (first_call) {
			result = glGetAttribLocation(dflt_program(), "position");
			if (result == -1) {
				throw std::runtime_error{"\"position\" is not a valid GLSL program variable."};
			}
			first_call = false;
		}
		return result;
	}

	GLint vs_attr_texture_coords()
	{
		static bool first_call = true;
		static GLint result = -1;
		if (first_call) {
			result = glGetAttribLocation(dflt_program(), "texture_coords");
			if (result == -1) {
				throw std::runtime_error{"\"texture_coords\" is not a valid GLSL program variable."};
			}
			first_call = false;
		}
		return result;
	}

	GLint vs_attr_color_factor()
	{
		static bool first_call = true;
		static GLint result = -1;
		if (first_call) {
			result = glGetAttribLocation(dflt_program(), "color_factor");
			if (result == -1) {
				throw std::runtime_error{"\"color_factor\" is not a valid GLSL program variable."};
			}
			first_call = false;
		}
		return result;
	}

	Renderer& renderer() { return *_renderer; }
	void renderer(std::unique_ptr<Renderer> renderer)
	{
		bool initializing = _renderer == nullptr;
		_renderer = std::move(renderer);
		if (!initializing && _renderer != nullptr) {
			// Reassigning the renderer. Need to invalidate shared textures and refresh all renderable objects.
			texture_manager().clear_cache();
			Renderable::refresh_all();
		}
	}

	Window& window() { return *_window; }
	void window(std::unique_ptr<Window> window)
	{
		_window = std::move(window);
	}

	Input& input() 
	{
		static Input _input;
		return _input;
	}

	ResourceManager<Texture>& texture_manager()
	{
		static ResourceManager<Texture> _texture_manager;
		return _texture_manager;
	}

	ResourceManager<Font>& font_manager()
	{
		static ResourceManager<Font> _font_manager;
		return _font_manager;
	}

	ResourceManager<Sound>& sound_manager()
	{
		static ResourceManager<Sound> _sound_manager;
		return _sound_manager;
	}
}
