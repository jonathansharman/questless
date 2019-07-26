//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "scene.hpp"

#include "ui/view_space.hpp"
#include "utility/reference.hpp"

#include "rsrc/splash.hpp"

namespace ql {
	namespace rsrc {
		struct fonts;
	}

	namespace scenes {
		//! The splash screen.
		struct splash : scene {
			splash(sf::RenderWindow& window, rsrc::fonts const& fonts);

		private:
			bool _sound_played = false;
			std::vector<view::point> _flame_positions{};

			rsrc::splash _rsrc;

			update_result scene_subupdate(sec elapsed_time, std::vector<sf::Event>& events) final;

			void scene_subdraw(sf::RenderTarget& target, sf::RenderStates states) const final;
		};
	}
}
