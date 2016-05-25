/**
* @file    TileTexturer.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the TileTexturer class.
*/

#include "animation/TileTexturer.h"
#include "sdl-wrappers/resources.h"

using namespace sdl;

namespace questless
{
	void TileTexturer::visit(const EdgeTile&)
	{
		_texture = Texture::make("resources/textures/terrain/tile.png", sdl::renderer(), SDL_BLENDMODE_BLEND);
		_texture->color(Color::black());
	}
	void TileTexturer::visit(const StoneTile&)
	{
		_texture = Texture::make("resources/textures/terrain/tile.png", sdl::renderer(), SDL_BLENDMODE_BLEND);
		_texture->color(Color::gray());
	}
	void TileTexturer::visit(const DirtTile&)
	{
		_texture = Texture::make("resources/textures/terrain/tile.png", sdl::renderer(), SDL_BLENDMODE_BLEND);
		_texture->color(Color::brown());
	}
	void TileTexturer::visit(const GrassTile&)
	{
		_texture = Texture::make("resources/textures/terrain/tile.png", sdl::renderer(), SDL_BLENDMODE_BLEND);
		_texture->color(Color::green());
	}
	void TileTexturer::visit(const WaterTile&)
	{
		_texture = Texture::make("resources/textures/terrain/tile.png", sdl::renderer(), SDL_BLENDMODE_BLEND);
		_texture->color(Color::blue());
	}
	void TileTexturer::visit(const SnowTile&)
	{
		_texture = Texture::make("resources/textures/terrain/tile.png", sdl::renderer(), SDL_BLENDMODE_BLEND);
		_texture->color(Color::white());
	}
}
