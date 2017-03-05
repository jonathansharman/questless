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
	Renderer& renderer();
	void renderer(std::unique_ptr<Renderer> renderer);

	Window& window();
	void window(std::unique_ptr<Window> window);

	Input& input();

	ResourceManager<Texture>& texture_manager();
	ResourceManager<Font>& font_manager();
	ResourceManager<Sound>& sound_manager();
}
