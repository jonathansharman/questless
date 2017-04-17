//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "sdl/resources.h"
#include "sdl/Renderable.h"

namespace sdl
{
	namespace
	{
		std::unique_ptr<Window> _window = nullptr;
		std::unique_ptr<Renderer> _renderer = nullptr;
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
