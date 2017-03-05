/**
* @file    TileTexturer.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the TileTexturer class.
*/

#include "animation/TileTexturer.h"
#include "sdl/resources.h"

using std::make_unique;

using namespace sdl;

namespace questless
{
	void TileTexturer::visit(EdgeTile const&)
	{
		_texture = make_unique<Texture>("resources/textures/terrain/tile.png");
		_texture->color(Color::black());
	}
	void TileTexturer::visit(StoneTile const&)
	{
		_texture = make_unique<Texture>("resources/textures/terrain/tile.png");
		_texture->color(Color::gray());
	}
	void TileTexturer::visit(DirtTile const&)
	{
		_texture = make_unique<Texture>("resources/textures/terrain/tile.png");
		_texture->color(Color::brown());
	}
	void TileTexturer::visit(GrassTile const&)
	{
		_texture = make_unique<Texture>("resources/textures/terrain/tile.png");
		_texture->color(Color::green());
	}
	void TileTexturer::visit(WaterTile const&)
	{
		_texture = make_unique<Texture>("resources/textures/terrain/tile.png");
		_texture->color(Color::blue());
	}
	void TileTexturer::visit(SnowTile const&)
	{
		_texture = make_unique<Texture>("resources/textures/terrain/tile.png");
		_texture->color(Color::white());
	}
}
