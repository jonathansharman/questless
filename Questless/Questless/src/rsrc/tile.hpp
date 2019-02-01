//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility.hpp"

#include "animation/sprite_animation.hpp"

#include <SFML/Graphics.hpp>

namespace ql::rsrc {
	//! Contains textures for tile animations.
	struct tile {
		struct {
			sf::Texture selector = load<sf::Texture>("resources/textures/terrain/selector.png");
		} ss;

		struct {
			sprite_animation selector;
		} ani;

		struct {
			sf::Texture blank = load<sf::Texture>("resources/textures/terrain/blank.png");
			sf::Texture dirt = load<sf::Texture>("resources/textures/terrain/dirt.png");
			sf::Texture grass = load<sf::Texture>("resources/textures/terrain/grass.png");
			sf::Texture sand = load<sf::Texture>("resources/textures/terrain/sand.png");
			sf::Texture snow = load<sf::Texture>("resources/textures/terrain/snow.png");
			sf::Texture stone = load<sf::Texture>("resources/textures/terrain/stone.png");
			sf::Texture water = load<sf::Texture>("resources/textures/terrain/water.png");
		} txtr;

		tile()
			: ani{{sprite_sheet{ss.selector, {4, 1}},
				  std::vector<sprite_animation::frame>{//
					  {0.100_s, {0, 0}, {0, 0}},
					  {0.075_s, {1, 0}, {0, 0}},
					  {0.050_s, {2, 0}, {0, 0}},
					  {0.025_s, {3, 0}, {0, 0}},
					  {0.050_s, {2, 0}, {0, 0}},
					  {0.075_s, {1, 0}, {0, 0}}},
				  sprite_animation::loop_type::looping}} {}
	};
}
