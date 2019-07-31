//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "rsrc/fonts.hpp"
#include "utility/reference.hpp"

#include <SFML/Graphics.hpp>

namespace ql {
	namespace scenes {
		struct scene;
	}

	//! Represents an instance of the game Questless.
	struct game {
		//! @param fullscreen Whether to run the game in fullscreen mode.
		game(bool fullscreen);

		~game();

		//! Runs a new game of Questless.
		void run();

	private:
		enum class state { being_editor }; //! @todo Should not be needed. Replace with scene.

		sf::RenderWindow _window;

		rsrc::fonts _fonts;
		
		uptr<scenes::scene> _scene;
	};
}
