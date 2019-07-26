//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "scene.hpp"

#include "ui/digraph_menu.hpp"
#include "ui/layout.hpp"
#include "utility/reference.hpp"

namespace ql::scenes {
	//! The scene for the main menu.
	struct main_menu : scene {
		main_menu(sf::RenderWindow& window, rsrc::fonts const& fonts);

	private:
		layout _layout;
		digraph_menu _main_menu;

		update_result scene_subupdate(sec elapsed_time, std::vector<sf::Event>& events) final;

		void scene_subdraw(sf::RenderTarget& target, sf::RenderStates states) const final;
	};
}
