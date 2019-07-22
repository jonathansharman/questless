//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "scene.hpp"

#include "ui/hud.hpp"
#include "utility/reference.hpp"

namespace ql::scene {
	//! The scene for an active game.
	struct playing : scene {
		playing(sf::Window& window, rsrc::fonts const& fonts);

	private:
		sf::View _view;

		hud _hud;

		std::optional<ent> _opt_player_id = std::nullopt;

		ent _region_id;

		// UI

		hud _hud;

		sf::Texture _txt_hex_highlight;
		sf::Texture _txt_hex_circle;

		update_result scene_subupdate(sec elapsed_time, input_manager& im) final;

		void scene_subdraw(sf::RenderTarget& target, sf::RenderStates states) const final;

		void spawn_player();
	};
}
