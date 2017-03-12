/**
* @file    Media.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Provides access to shared resources.
*/

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
	/// @return The renderer.
	////
	Renderer& renderer();

	/// Sets the renderer to the given value.
	////
	void renderer(std::unique_ptr<Renderer> renderer);

	/// @return The application window.
	////
	Window& window();

	/// Sets the window to the given value.
	////
	void window(std::unique_ptr<Window> window);

	/// @return The input object.
	////
	Input& input();

	/// @return The texture manager.
	////
	ResourceManager<Texture>& texture_manager();
	using TextureHandle = ResourceManager<Texture>::Handle;

	/// @return The font manager.
	////
	ResourceManager<Font>& font_manager();
	using FontHandle = ResourceManager<Font>::Handle;

	/// @return The sound manager.
	////
	ResourceManager<Sound>& sound_manager();
	using SoundHandle = ResourceManager<Sound>::Handle;
}
