//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "scene.hpp"

#include "animation/world_renderer.hpp"
#include "animation/camera.hpp"
#include "ui/hud.hpp"
#include "utility/reference.hpp"

namespace ql::scene {
	//! The scene for an active game.
	class playing : scene {
	public:
		playing();
	private:
		ql::camera _camera;

		std::optional<ql::id<being>> _player_being_id = std::nullopt;
		player* _player = nullptr;

		uptr<ql::region> _region;

		sdl::font _fnt_20pt;

		uptr<ql::world_renderer> _world_renderer;

		// UI

		uptr<sdl::texture> _txt_test1;
		uptr<sdl::texture> _txt_test2;
		uptr<sdl::texture> _txt_test3;
		uptr<sdl::texture> _txt_test_even;

		uptr<sdl::texture> _txt_hex_highlight;
		uptr<sdl::texture> _txt_hex_circle;

		units::game_space::point _point_clicked_rounded{0.0, 0.0};

		update_result subupdate() final;

		void subdraw() final;

		void spawn_player();

		void update_view();
	};
}
