//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/TileTexturer.h"
#include "sdl/resources.h"

using std::make_unique;

using namespace sdl;

namespace questless
{
	void TileTexturer::visit(DirtTile const&)
	{
		_texture = make_unique<Texture>("resources/textures/terrain/dirt.png");
	}
	void TileTexturer::visit(EdgeTile const&)
	{
		_texture = make_unique<Texture>("resources/textures/terrain/tile.png");
		_texture->color(Color::black());
	}
	void TileTexturer::visit(GrassTile const&)
	{
		_texture = make_unique<Texture>("resources/textures/terrain/grass.png");
	}
	void TileTexturer::visit(SandTile const&)
	{
		_texture = make_unique<Texture>("resources/textures/terrain/sand.png");
	}
	void TileTexturer::visit(SnowTile const&)
	{
		_texture = make_unique<Texture>("resources/textures/terrain/snow.png");
	}
	void TileTexturer::visit(StoneTile const&)
	{
		_texture = make_unique<Texture>("resources/textures/terrain/stone.png");
	}
	void TileTexturer::visit(WaterTile const&)
	{
		_texture = make_unique<Texture>("resources/textures/terrain/water.png");
	}
}
