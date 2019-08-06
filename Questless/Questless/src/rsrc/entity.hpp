//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility.hpp"

#include "animation/still_image.hpp"

#include <SFML/Graphics.hpp>

namespace ql::rsrc {
	//! Contains the textures used to animate entities.
	struct entity {
		struct {
			sf::Texture unknown = load<sf::Texture>("resources/textures/entities/unknown.png");

			// Objects

			sf::Texture firewood = load<sf::Texture>("resources/textures/entities/objects/firewood.png");
			sf::Texture item_box = load<sf::Texture>("resources/textures/entities/objects/item_box.png");
			sf::Texture grave = load<sf::Texture>("resources/textures/entities/objects/grave.png");
		} txtr;

		struct {
			// Beings

			sf::Texture goblin = load<sf::Texture>("resources/textures/entities/beings/goblin.png");
			sf::Texture human = load<sf::Texture>("resources/textures/entities/beings/human.png");
		} ss;
	};
}
