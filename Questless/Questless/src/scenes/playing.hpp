//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "scene.hpp"

#include "animation/world_renderer.hpp"
#include "ui/hud.hpp"
#include "utility/reference.hpp"

namespace ql::scene {
	//! The scene for an active game.
	struct playing : scene {
		playing();
	private:
		sf::View _view;

		std::optional<ql::id<being>> _player_being_id = std::nullopt;
		player* _player = nullptr;

		uptr<ql::region> _region;

		sf::Font _fnt_20pt;

		uptr<ql::world_renderer> _world_renderer;

		// UI

		sf::Texture _txt_test1;
		sf::Texture _txt_test2;
		sf::Texture _txt_test3;
		sf::Texture _txt_test_even;

		sf::Texture _txt_hex_highlight;
		sf::Texture _txt_hex_circle;

		world::point _point_clicked_rounded;

		update_result scene_subupdate(sec elapsed_time) final;

		void scene_subdraw() final;

		void spawn_player();

		void update_view();
	};
}
