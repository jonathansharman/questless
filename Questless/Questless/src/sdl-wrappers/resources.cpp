/**
* @file    Media.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the shared resource functions.
*/

#include "sdl-wrappers/resources.h"
#include "sdl-wrappers/Renderable.h"

namespace sdl
{
	namespace
	{
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
