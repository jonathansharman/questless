//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "playing.hpp"

#include "game.hpp"
#include "utility/reference.hpp"
#include "world/region.hpp"
#include "world/world.hpp"
//! @todo The following are needed only for player spawning. Perhaps this should be the responsibility of a different class.
#include "agents/agent.hpp"
#include "agents/player.hpp"
#include "entities/beings/goblinoids/goblin.hpp"
#include "entities/beings/human.hpp"
#include "items/magic/gatestone.hpp"
#include "items/scroll.hpp"
#include "items/weapons/arrow.hpp"
#include "items/weapons/bow.hpp"
#include "items/weapons/quarterstaff.hpp"
#include "items/weapons/quiver.hpp"
#include "magic/spell.hpp"

#include <fmt/format.h>

using namespace media;
using namespace vecx;
using namespace vecx::literals;

namespace ql::scene {
	playing::playing() : _fnt_20pt{"resources/fonts/firamono.ttf", 20} {
		_region = umake<ql::region>("Region1");
		//_region = umake<region>("Slot1", "Region1");

		spawn_player();
		// Update the player's initial world view.
		_player->update_view();
		// Initialize the world renderer.
		_world_renderer = umake<ql::world_renderer>(_player->world_view());
		// Set the view relative to the player's being.
		_view.setCenter(to_sfml(to_world(the_game().beings.cref(*_player_being_id).coords)));
	}

	update_result playing::scene_subupdate(sec elapsed_time) {
		constexpr tick ticks_per_turn = 10_tick;

		// Update HUD.
		_hud->update();

		// Update dialogs.
		if (!_dialogs.empty()) {
			_dialogs.front()->update();
			if (_dialogs.front()->closed()) {
				_dialogs.pop_front();

				if (_dialogs.empty()) {
					// End of player's turn.
				}
			}
		} else if (_player_action_dialog) {
			_player_action_dialog->update();
			if (_player_action_dialog->closed()) { _player_action_dialog = nullptr; }
		} else {
			// Take turns.

			// Work through the beings ready to take their turns, until all have acted or one of them can't finish
			// acting yet.
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
					// Awaiting player input to complete current action. Stop taking turns, and start at the next agent
					// once this action is complete.
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
		_world_renderer->update(elapsed_time);

		// Disable camera controls during dialogs (except player action dialog).
		if (_dialogs.empty()) {
			float const zoom = INSERT_WINDOW_WIDTH_HERE / _view.getViewport().width;
			// Pan camera.
			if (im.down(sf::Mouse::Middle)) {
				auto const world_mouse_movement = the_window().mapPixelToCoords(im.mouse_movement());
				_view.move(world_mouse_movement / zoom);
			}
			constexpr double pan_amount = 10.0;
			if (im.down(sf::Keyboard::Numpad8)) { _view.move(sf::Vector2f{0.0, pan_amount} / zoom); }
			if (im.down(sf::Keyboard::Numpad4)) { _view.move(sf::Vector2f{-pan_amount, 0.0} / zoom); }
			if (im.down(sf::Keyboard::Numpad2)) { _view.move(sf::Vector2f{0.0, -pan_amount} / zoom); }
			if (im.down(sf::Keyboard::Numpad6)) { _view.move(sf::Vector2f{pan_amount, 0.0} / zoom); }

			// Scale camera.
			_view.zoom(1.0 + im.scroll() / 10.0);

			// Rotate camera.
			degrees angle = degrees{_view.getRotation()};
			if (im.down(sf::Keyboard::Numpad9)) {
				_view.rotate((-3.0_deg - angle / 20.0).value);
			} else if (im.down(sf::Keyboard::Numpad7)) {
				_view.rotate((3.0_deg - angle / 20.0).value);
			}
		}

		// Camera follow.
		static bool follow = true; //! @todo Should not be static.
		if (im.press_count(sf::Keyboard::Backspace) & 1) { // Odd number of presses
			follow = !follow;
		}
		if (follow) {
			if (being* player_being = beings.ptr(*_player_being_id)) {
				constexpr double recoil = 0.2;

				game_space::vector to_player = to_world(player_being->coords) - _camera.position;
				_camera.position = _camera.position + recoil * to_player;

				double to_zoom_1 = 1.0 - _camera.zoom;
				_camera.zoom += recoil * to_zoom_1;
				if (abs(_camera.zoom - 1.0) < 0.001) { _camera.zoom = 1.0; }

				// Only snap camera angle back if the player isn't actively rotating it.
				if (!im.any_of_down({sf::Keyboard::Numpad9, sf::Keyboard::Numpad7})) { _camera.angle *= 1 - recoil; }
			}
		}
		return update_result::continue_game;
	};

	void playing::scene_subdraw() {
		_world_renderer->draw_terrain();

		if (im.pressed(sf::Mouse::Right)) { _point_clicked_rounded = to_world(_camera.tile_hovered()); }
		_camera.draw(
			*_txt_hex_highlight, to_world(_camera.tile_hovered()), texture_space::vector::zero(), colors::white_vector(0.5f));
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
			sf::Text txt_cam_coords;
			txt_cam_coords.setFont(_fnt_20pt);
			txt_cam_coords.setFillColor(sf::Color::White);
			txt_cam_coords.setString(fmt::format("Cam: (({:.2f}, {:.2f}), {:.2f}, {:.2f})",
				_view.getCenter().x,
				_view.getCenter().y,
				_camera.angle.value().count(),
				_camera.zoom));
			the_window().draw(txt_cam_coords);
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
			_camera.draw_lines({to_world(origin), to_world(q_axis)}, sf::Color::Green);
			_camera.draw_lines({to_world(origin), to_world(r_axis)}, sf::Color::Red);
		}

		_txt_test1->draw(window_space::point{0, 0});
		the_renderer().draw_box(
			window_space::box{window_space::point{0, 0}, window_space::vector{15, 15}}, 1, colors::blue(), colors::clear());
	}

	void playing::spawn_player() {
		auto player_being = umake<human>(agent::make<player>);
		_player = dynamic_cast<player*>(&player_being->agent());
		_player_being_id = player_being->id;
		player_being->inventory.add(items.add(umake<scroll>(umake<magic::shock>())).id);
		player_being->inventory.add(items.add(umake<scroll>(umake<magic::heal>())).id);
		player_being->inventory.add(items.add(umake<scroll>(umake<magic::teleport>())).id);
		player_being->inventory.add(items.add(umake<scroll>(umake<magic::telescope>())).id);
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
			_world_renderer->update_view(_player->world_view(), player_being->agent()->poll_perceived_effects());
		}
	}
}
