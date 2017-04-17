//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Provides access to shared resources.

#pragma once

#include <memory>

#include "ResourceManager.h"
#include "Renderer.h"
#include "Window.h"
#include "Input.h"
#include "Texture.h"
#include "Font.h"
#include "Sound.h"

namespace sdl
{
	//! The renderer.
	Renderer& renderer();

	//! Sets the renderer to the given value.
	void renderer(std::unique_ptr<Renderer> renderer);

	//! The application window.
	Window& window();

	//! Sets the window to the given value.
	void window(std::unique_ptr<Window> window);

	//! The input object.
	Input& input();

	//! The texture manager.
	ResourceManager<Texture>& texture_manager();
	using TextureHandle = ResourceManager<Texture>::Handle;

	//! The font manager.
	ResourceManager<Font>& font_manager();
	using FontHandle = ResourceManager<Font>::Handle;

	//! The sound manager.
	ResourceManager<Sound>& sound_manager();
	using SoundHandle = ResourceManager<Sound>::Handle;
}
