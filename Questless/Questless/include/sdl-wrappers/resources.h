/**
* @file    Media.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Provides access to shared resources.
*/

#ifndef MEDIA_H
#define MEDIA_H

#include <memory>

#include "sdl-wrappers/Renderer.h"
#include "sdl-wrappers/ResourceManager.h"
#include "sdl-wrappers/Texture.h"
#include "sdl-wrappers/Font.h"
#include "sdl-wrappers/Sound.h"

namespace sdl
{
	Renderer& renderer();
	void renderer(std::unique_ptr<Renderer> renderer);

	ResourceManager<Texture>& texture_manager();
	ResourceManager<Font>& font_manager();
	ResourceManager<Sound>& sound_manager();
}

#endif
