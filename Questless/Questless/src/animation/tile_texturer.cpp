//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/tile_texturer.hpp"
#include "sdl/resources.hpp"

namespace ql {
	void tile_texturer::visit(dirt_tile const&) {
		_texture = umake<sdl::texture>("resources/textures/terrain/dirt.png");
	}
	void tile_texturer::visit(edge_tile const&) {
		_texture = umake<sdl::texture>("resources/textures/terrain/tile.png"); //! Edge tiles should simply not be drawn.
	}
	void tile_texturer::visit(grass_tile const&) {
		_texture = umake<sdl::texture>("resources/textures/terrain/grass.png");
	}
	void tile_texturer::visit(sand_tile const&) {
		_texture = umake<sdl::texture>("resources/textures/terrain/sand.png");
	}
	void tile_texturer::visit(snow_tile const&) {
		_texture = umake<sdl::texture>("resources/textures/terrain/snow.png");
	}
	void tile_texturer::visit(stone_tile const&) {
		_texture = umake<sdl::texture>("resources/textures/terrain/stone.png");
	}
	void tile_texturer::visit(water_tile const&) {
		_texture = umake<sdl::texture>("resources/textures/terrain/water.png");
	}
}
