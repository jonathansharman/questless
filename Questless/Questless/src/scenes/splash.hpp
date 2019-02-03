//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "scene.hpp"

#include "utility/reference.hpp"

#include "rsrc/splash.hpp"

namespace ql::scene {
	//! The splash screen scene.
	struct splash : scene {
		splash();

	private:
		bool _sound_played = false;
		std::vector<sf::Vector2i> _flame_positions{};

		rsrc::splash _rsrc;

		update_result scene_subupdate(sec elapsed_time, input_manager& im) final;

		void scene_subdraw() final;
	};
}
