//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "scene.hpp"

#include "ui/window_widget.hpp"
#include "utility/reference.hpp"

namespace ql::scenes {
	//! The scene for the main menu.
	struct main_menu : scene {
		main_menu(view::vector window_size, rsrc::fonts const& fonts);

	private:
		view::vector _window_size;

		update_result scene_subupdate(sec elapsed_time, std::vector<sf::Event>& events) final;

		void scene_subdraw(sf::RenderTarget& target, sf::RenderStates states) const final;
	};
}
