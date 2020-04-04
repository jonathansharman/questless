//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "menu_fwd.hpp"
#include "utility.hpp"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

namespace ql::rsrc {
	//! Contains menu resources.
	struct menu {
		struct {
			sf::Texture ul = load<sf::Texture>("resources/textures/menu/ul.png");
			sf::Texture ur = load<sf::Texture>("resources/textures/menu/ur.png");
			sf::Texture dl = load<sf::Texture>("resources/textures/menu/dl.png");
			sf::Texture dr = load<sf::Texture>("resources/textures/menu/dr.png");
			sf::Texture u = load<sf::Texture>("resources/textures/menu/u.png");
			sf::Texture d = load<sf::Texture>("resources/textures/menu/d.png");
			sf::Texture l = load<sf::Texture>("resources/textures/menu/l.png");
			sf::Texture r = load<sf::Texture>("resources/textures/menu/r.png");
			sf::Texture tile = load<sf::Texture>("resources/textures/menu/tile.png");
		} txtr;

		struct {
			sf::SoundBuffer hover = load<sf::SoundBuffer>("resources/sounds/menu/hover.wav");
			sf::SoundBuffer select = load<sf::SoundBuffer>("resources/sounds/menu/select.wav");
		} sfx;
	};
}
