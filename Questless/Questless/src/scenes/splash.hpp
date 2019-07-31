//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "scene.hpp"

#include "ui/view_space.hpp"
#include "ui/window_widget.hpp"
#include "utility/reference.hpp"

#include "rsrc/splash.hpp"

namespace ql {
	namespace rsrc {
		struct fonts;
	}

	namespace scenes {
		//! The UI for the splash screen.
		struct splash_widget : widget {
			splash_widget(widget& parent);

			view::vector get_size() const final;

			void update(sec elapsed_time, std::vector<sf::Event>& events) final;

			void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

			//! Whether the player wants to skip the scene.
			bool skip() const {
				return _skip;
			}

		private:
			rsrc::splash _rsrc;

			bool _sound_played = false;

			bool _skip = false;

			std::vector<vecx::vector<cancel::unitless<float>, 2>> _flame_positions{};
		};

		//! The splash screen scene.
		struct splash : scene {
			splash(view::vector window_size, rsrc::fonts const& fonts);

		private:
			window_widget _root;
			splash_widget _splash_widget;
			sf::Shader _fade_shader;

			update_result scene_subupdate(sec elapsed_time, std::vector<sf::Event>& events) final;

			void scene_subdraw(sf::RenderTarget& target, sf::RenderStates states) const final;
		};
	}
}
