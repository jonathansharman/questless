//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "playing.hpp"

#include "world/region.hpp"
//! @todo The following are needed only for player spawning. Perhaps this should be the responsibility of a different class.
#include "agents/agent.hpp"
#include "agents/player.hpp"
#include "items/magic/gatestone.hpp"
#include "entities/beings/goblinoids/goblin.hpp"
#include "entities/beings/human.hpp"
#include "items/scroll.hpp"
#include "items/weapons/arrow.hpp"
#include "items/weapons/bow.hpp"
#include "items/weapons/quarterstaff.hpp"
#include "items/weapons/quiver.hpp"
#include "magic/all_spells.hpp"

#include "sdl/resources.hpp"

using namespace sdl;
using namespace units;

namespace ql::scene {
	playing::playing()
		: _camera{game_space::point{0.0, 0.0}}
		, _fnt_20pt{"resources/fonts/firamono.ttf", 20, SDL_BLENDMODE_BLEND}
	{
		_region = umake<ql::region>("Region1");
		//_region = umake<region>("Slot1", "Region1");

		spawn_player();
		// Create the HUD and pass the player's being ID to it.
		_hud = umake<ql::hud>();
		_hud->set_player_being_id(*_player_being_id);
		// Update the player's initial world view.
		_player->update_world_view();
		// Initialize the world renderer.
		_world_renderer = umake<ql::world_renderer>(_player->world_view());
		// Set the camera position relative to the player's being.
		_camera.position(game_space::point{to_world(beings.cref(*_player_being_id).coords)});
	}

	update_result playing::subupdate() {
		constexpr tick ticks_per_turn = 10_tick;

		// Update camera.
		_camera.update();

		// Update HUD.
		_hud->update();

		// Update dialogs.
		if (!_dialogs.empty()) {
			if (_dialogs.front()->update() == dialog::state::closed) {
				_dialogs.pop_front();

				if (_dialogs.empty()) {
					// End of player's turn.
				}
			}
		} else if (_player_action_dialog) {
			if (_player_action_dialog->update() == dialog::state::closed) {
				_player_action_dialog = nullptr;
			}
		} else {
			// Take turns.

			// Work through the beings ready to take their turns, until all have acted or one of them can't finish acting yet.
			while (being* next_ready_being = _region->next_ready_being()) {
				if (next_ready_being->id == _player_being_id) {
					// Update the player view before the player acts.
					_player->update_view();
				}
				[[maybe_unused]] auto complete = next_ready_being->act();
				if (next_ready_being->id == _player_being_id) {
					// Update the player view after the player acts.
					_player->update_view();
				}

				if (_player_action_dialog || !_dialogs.empty()) {
					// Awaiting player input to complete current action. Stop taking turns, and start at the next agent once this action is complete.
					break;
				}
			}
			if (!_player_action_dialog && _dialogs.empty()) {
				// Update the region.
				_region->update(ticks_per_turn);
				// Update the player view after each time unit passes.
				_player->update_view();
			}
		}

		// Update world renderer.
		_world_renderer->update();

		// Disable camera controls during dialogs (except player action dialog).
		if (_dialogs.empty()) {
			// Pan camera.
			if (the_input().down(mouse_button::middle)) {
				window_space::vector const mouse_shift = the_input().last_mouse_position() - the_input().mouse_position();
				_camera.position += game_space::vector{static_cast<double>(mouse_shift.x()), static_cast<double>(-mouse_shift.y())}
					.rotated(_camera.angle.value()) / _camera.zoom.value();
			}
			double const pan_amount = 10.0;
			if (the_input().down(SDLK_KP_8)) {
				_camera.position += game_space::vector{0.0, pan_amount} / _camera.zoom;
			}
			if (the_input().down(SDLK_KP_4)) {
				_camera.position += game_space::vector{-pan_amount, 0.0} / _camera.zoom;
			}
			if (the_input().down(SDLK_KP_2)) {
				_camera.position += game_space::vector{0.0, -pan_amount} / _camera.zoom;
			}
			if (the_input().down(SDLK_KP_6)) {
				_camera.position += game_space::vector{pan_amount, 0.0} / _camera.zoom;
			}

			// Scale camera.
			_camera.zoom *= 1.0 + the_input().scroll() / 10.0;

			// Rotate camera.
			game_space::radians angle = _camera.angle;
			if (the_input().down(SDLK_KP_9)) {
				_camera.angle += (-game_space::radians::circle() / 6.0 - angle) / 20.0;
			} else if (the_input().down(SDLK_KP_7)) {
				_camera.angle += (game_space::radians::circle() / 6.0 - angle) / 20.0;
			}
		}

		// Camera follow.
		static bool follow = true;
		if (the_input().presses(SDLK_BACKSPACE) & 1) { // Odd number of presses
			follow = !follow;
		}
		if (follow) {
			if (being* player_being = beings.ptr(*_player_being_id)) {
				constexpr double recoil = 0.2;

				game_space::vector to_player = to_world(player_being->coords) - _camera.position;
				_camera.position = _camera.position + recoil * to_player;

				double to_zoom_1 = 1.0 - _camera.zoom;
				_camera.zoom += recoil * to_zoom_1;
				if (abs(_camera.zoom - 1.0) < 0.001) {
					_camera.zoom = 1.0;
				}

				// Only snap camera angle back if the player isn't actively rotating it.
				if (!the_input().down(SDLK_KP_9) && !the_input().down(SDLK_KP_7)) {
					_camera.angle *= 1 - recoil;
				}
			}
		}
		return update_result::continue_game;
	};

	void playing::subdraw() {
		_world_renderer->draw_terrain();

		if (the_input().pressed(mouse_button::right)) {
			_point_clicked_rounded = to_world(_camera.tile_hovered());
		}
		_camera.draw(*_txt_hex_highlight, to_world(_camera.tile_hovered()), texture_space::vector::zero(), colors::white_vector(0.5f));
		_camera.draw(*_txt_hex_circle, _point_clicked_rounded);

		_world_renderer->draw_entities();
		_world_renderer->draw_effects();

		// Draw HUD.
		_hud->draw();

		// Draw dialogs.
		for (auto const& dialog : _dialogs) {
			dialog->draw();
		}

		{ // Draw camera position.
			std::ostringstream ss_cam_coords;
			ss_cam_coords.setf(std::ios::fixed);
			ss_cam_coords.precision(2);
			ss_cam_coords << "Cam: ((" << _camera.position.x() << ", " << _camera.position.y() << "), ";
			ss_cam_coords << _camera.angle.value().count() << ", " << _camera.zoom << ")";
			texture txt_cam_coords = _fnt_20pt.render(ss_cam_coords.str().c_str(), colors::white());
			txt_cam_coords.draw(window_space::point{0, 0});
		}
		{ // Draw camera hex position.
			auto cam_hex_coords = to_region_tile(_camera.position);
			std::ostringstream ss_cam_hex_coords;
			ss_cam_hex_coords << "Cam hex: (" << cam_hex_coords.q.value << ", " << cam_hex_coords.r.value << ")";
			texture txt_cam_hex_coords = _fnt_20pt.render(ss_cam_hex_coords.str().c_str(), colors::white());
			txt_cam_hex_coords.draw(window_space::point{0, 25});
		}
		{ // Draw time.
			std::ostringstream ss_time;
			tick const time_of_day = _region->time_of_day();
			std::string time_name;
			switch (_region->period_of_day()) {
			case period_of_day::morning:
				time_name = "Morning";
				break;
			case period_of_day::afternoon:
				time_name = "Afternoon";
				break;
			case period_of_day::dusk:
				time_name = "Dusk";
				break;
			case period_of_day::evening:
				time_name = "Evening";
				break;
			case period_of_day::night:
				time_name = "Night";
				break;
			case period_of_day::dawn:
				time_name = "Dawn";
				break;
			}
			ss_time << "Time: " << _region->time().value << " (" << time_of_day.value << ", " << time_name << ')';
			texture txt_turn = _fnt_20pt.render(ss_time.str().c_str(), colors::white());
			txt_turn.draw(window_space::point{0, 50});
		}

		{ // Draw q- and r-axes.
			region_tile::point origin{0, 0};
			region_tile::point q_axis{5, 0};
			region_tile::point r_axis{0, 5};
			_camera.draw_lines({to_world(origin), to_world(q_axis)}, colors::green());
			_camera.draw_lines({to_world(origin), to_world(r_axis)}, colors::red());
		}

		_txt_test1->draw(window_space::point{0, 0});
		the_renderer().draw_box(window_space::box{window_space::point{0, 0}, window_space::vector{15, 15}}, 1, colors::blue(), colors::clear());

		//! @todo Enable to test polygon rendering after adding support for non-convex polygons.
#if 0
		{ // Draw hexagon.
			view_space::polygon hexagon;
			for (int i = 0; i < 6; ++i) {
				auto const angle = constants::tau / 6 * i;
				auto const offset = view_space::vector{static_cast<float>(cos(angle) * 50), static_cast<float>(sin(angle) * 50)};
				hexagon.push_back(view_space::point{50.0f, 50.0f} +offset);
			}
			bool const hovering = view_space::contains(hexagon, to_view_space(the_input().mouse_position()));
			float border_width = abs(static_cast<float>(10 * sin(constants::tau * duration_cast<game_space::seconds>(clock::now() - _time_last_state_change).count() / 4)));
			the_renderer().draw_polygon(hexagon, border_width, colors::white(), hovering ? colors::teal() : colors::orange());

			// Reverse hexagon and draw again. (Testing winding-order detection.)
			auto reverse_polygon = view_space::polygon(hexagon.rbegin(), hexagon.rend());
			for (auto& v : reverse_polygon) { v.y() += 100.0f; }
			the_renderer().draw_polygon(reverse_polygon, border_width, colors::white(), colors::clear());
		}
		{ // Draw right triangles. (Different border widths and winding orders.)
			the_renderer().draw_polygon
				( view_space::polygon
					{ view_space::point{100.0f, 0.0f}
					, view_space::point{200.0f, 0.0f}
					, view_space::point{100.0f, 100.0f}
					}
				, 1.0f
				, colors::red()
				, colors::cyan()
			);
			the_renderer().draw_polygon
				( view_space::polygon
					{ view_space::point{100.0f, 100.0f}
					, view_space::point{100.0f, 200.0f}
					, view_space::point{200.0f, 100.0f}
					}
				, 5.0f
				, colors::cyan()
				, colors::red()
			);
		}
		{ // Draw non-convex polygon.
			float border_width = abs(static_cast<float>(20 * sin(constants::tau * duration_cast<game_space::seconds>(clock::now() - _time_last_state_change).count() / 4)));
			the_renderer().draw_polygon
				( view_space::polygon
					{ view_space::point{200.0f, 0.0f}
					, view_space::point{300.0f, 0.0f}
					, view_space::point{250.0f, 50.0f}
					, view_space::point{300.0f, 100.0f}
					, view_space::point{200.0f, 100.0f}
					}
				, border_width
				, colors::lime()
				, colors::magenta()
				);
		}
		{ // Draw a star outline.
			view_space::point const center{350.0f, 50.0f};
			view_space::polygon star;
			auto start_angle = 0;// constants::tau * duration_cast<GameSeconds>(clock::now() - _time_last_state_change).count() / 4;
			for (int i = 0; i < 10; ++i) {
				float const radius = i & 1 ? 20.0f : 50.0f;
				auto const angle = constants::tau / 10 * i + start_angle;
				auto const offset = view_space::vector{static_cast<float>(cos(angle) * radius), static_cast<float>(sin(angle) * radius)};
				star.push_back(center + offset);
			}
			float border_width = abs(static_cast<float>(8 * sin(constants::tau * duration_cast<game_space::seconds>(clock::now() - _time_last_state_change).count() / 4)));
			//the_renderer().draw_polygon(star_vertices, colors::lime(), Fill::outline);
			//the_renderer().draw_polygon(star_vertices, colors::lime(), Fill::solid);
			the_renderer().draw_polygon(star, border_width, colors::red(0.5f), colors::blue(0.5f));
		}
#endif
	}

	void playing::spawn_player() {
		auto player_being = umake<human>(agent::make<player>);
		_player = dynamic_cast<player*>(&player_being->agent());
		_player_being_id = player_being->id;
		player_being->inventory.add(items.add(umake<scroll>(umake<magic::shock>())).id);
		player_being->inventory.add(items.add(umake<scroll>(umake<magic::heal>())).id);
		player_being->inventory.add(items.add(umake<scroll>(umake<magic::teleport>())).id);
		player_being->inventory.add(items.add(umake<scroll>(umake<magic::eagle_eye>())).id);
		player_being->inventory.add(items.add(umake<quarterstaff>()).id);
		player_being->inventory.add(items.add(umake<bow>()).id);
		{ // Give the player a quiver of arrows.
			inventory inventory;
			constexpr int arrow_count = 20;
			for (int i = 0; i < arrow_count; ++i) {
				inventory.add(items.add(umake<arrow>()).id);
			}
			player_being->inventory.add(items.add(umake<quiver>(std::move(inventory))).id);
		}
		player_being->inventory.add(items.add(umake<gatestone>(100.0_mp, 100.0_mp, 0_tick, magic::color::green)).id);
		_region->spawn_player(std::move(player_being));
	}

	void playing::update_view() {
		//! @todo Do something reasonable with the player view when the player dies.
		being const* player_being = beings.cptr(*_player_being_id);
		if (player_being && player_being->mortality != ql::mortality::dead) {
			// Update the player's world view.
			_player->update_view();
			// Update the world renderer's world view.
			_world_renderer->update_view(_player->world_view(), _player->poll_perceived_effects());
		}
	}
}
