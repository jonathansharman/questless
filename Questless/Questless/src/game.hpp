//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "ui/input_manager.hpp"
#include "utility/reference.hpp"

#include <SFML/Graphics.hpp>

namespace ql {
	struct being;
	struct item;
	struct light_source;
	struct object;
	namespace scene {
		class scene;
	}

	//! Represents an instance of the game Questless.
	struct game {
		~game();

		//! Runs a new game of Questless.
		void run();

	private:
		enum class state { being_editor }; //! @todo Should not be needed. Replace with scene.

		sf::RenderWindow _window;

		input_manager _im{_window};

		uptr<scene::scene> _scene;

		//! @param fullscreen Whether to run the game in fullscreen mode.
		game(bool fullscreen);

		void load_textures();
	};
}
