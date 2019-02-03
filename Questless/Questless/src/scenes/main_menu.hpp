//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "scene.hpp"

#include "ui/digraph_menu.hpp"
#include "utility/reference.hpp"

namespace ql::scene {
	//! The splash screen scene.
	struct main_menu : scene {
		main_menu();
	private:
		digraph_menu _main_menu{480, 640};

		update_result subupdate() final;

		void subdraw() final;
	};
}
