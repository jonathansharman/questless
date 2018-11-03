//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "main_menu.hpp"

using namespace sdl;
using namespace units;

namespace ql::scene {
	main_menu::main_menu() {
		// Add pages.
		_main_menu.add_page("Questless");
		_main_menu.add_page("Settings");
		// Add options.
		_main_menu.add_option("Questless", "Continue");
		_main_menu.add_option("Questless", "Begin Anew");
		_main_menu.add_option("Questless", "Settings", "Settings");
		_main_menu.add_option("Questless", "Being Editor");
		_main_menu.add_option("Questless", "Quit");
		_main_menu.add_option("Settings", "Save", "Questless");
		_main_menu.add_option("Settings", "Cancel", "Questless");
	}

	update_result main_menu::subupdate() {
		_main_menu.update();
		for (auto const&[page, option] : _main_menu.poll_selections()) {
			if (page == "Questless") {
				if (option == "Continue" || option == "Begin Anew") {
					_state = state::playing;
				} else if (option == "Being Editor") {
					_state = state::being_editor;
				} else if (option == "Quit") {
					return update_result::game_over;
				}
			}
		}
		return update_result::continue_game;
	};

	void main_menu::subdraw() {
		_main_menu.draw(the_window().window_center(), window_space::align_center, window_space::align_middle);
	}
}
