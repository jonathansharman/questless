//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "scene.hpp"

#include "ui/hud.hpp"
#include "ui/layout.hpp"
#include "utility/reference.hpp"

namespace ql::scenes {
	//! The scene for an active game.
	struct playing : scene {
		playing(sf::RenderWindow& window, rsrc::fonts const& fonts, ent region_id);

	private:
		sf::View _view;

		ent _region_id;

		ent _player_id;

		layout _layout;
		hud _hud;

		sf::Texture _txt_hex_highlight;
		sf::Texture _txt_hex_circle;

		update_result scene_subupdate(sec elapsed_time, std::vector<sf::Event>& events) final;

		void scene_subdraw(sf::RenderTarget& target, sf::RenderStates states) const final;
	};
}
