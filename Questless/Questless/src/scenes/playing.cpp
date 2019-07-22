//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "playing.hpp"

#include "utility/reference.hpp"
#include "world/region.hpp"
#include "world/world_space.hpp"
//! @todo The following are needed only for player spawning. Perhaps this should be the responsibility of a different class.
#include "agents/agent.hpp"
#include "agents/player.hpp"
#include "entities/beings/human.hpp"
#include "items/magic/gatestone.hpp"
#include "items/magic/scroll.hpp"
#include "items/weapons/arrow.hpp"
#include "items/weapons/bow.hpp"
#include "items/weapons/quarterstaff.hpp"
#include "items/weapons/quiver.hpp"
#include "magic/spell.hpp"
#include "rsrc/fonts.hpp"

#include <fmt/format.h>

using namespace vecx;
using namespace vecx::literals;

namespace ql::scene {
	playing::playing(sf::Window& window, rsrc::fonts const& fonts) : scene{window, fonts} {
		_region_id = reg.create();
		reg.assign<region>(_region_id, "Region1");

		spawn_player();
		// Initialize the world renderer.
		_world_renderer = umake<ql::world_renderer>(_player->world_view());
		// Set the view relative to the player's being.
		_view.setCenter(to_sfml(to_world(the_game().beings.cref(*_player_being_id).coords)));
	}

	update_result playing::scene_subupdate(sec elapsed_time, input_manager& im) {
		// Update HUD.
		_hud.update(elapsed_time, im);

		// Update dialogs.
		if (!_dialogs.empty()) {
			_dialogs.front()->update();
			if (_dialogs.front()->closed()) {
				_dialogs.pop_front();

				if (_dialogs.empty()) {
					// End of player's turn.
				}
			}
		} else {
			// Take turns.

			// Work through the beings ready to take their turns, until all have acted or one of them can't finish
			// acting yet.
			while (being* next_ready_being = _region->next_ready_being()) {
				if (next_ready_being->id == _player_id) {
					// Update the player view before the player acts.
					_player->update_view();
				}
				next_ready_being->act();
				if (next_ready_being->id == _player_id) {
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

		// Disable camera controls during dialogs (except player action dialog).
		if (_dialogs.empty()) {
			float const zoom = _window.getSize().x / _view.getViewport().width;
			// Pan camera.
			if (im.down(sf::Mouse::Middle)) {
				auto const world_mouse_movement = _window.mapPixelToCoords(im.mouse_movement());
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
	}

	void playing::scene_subdraw(sf::RenderTarget& target, sf::RenderStates states) const {
		// Draw HUD.
		_hud.draw(target, states);

		// Draw dialogs.
		for (auto const& dialog : _dialogs) {
			dialog->draw();
		}

		{ // Draw time.
			auto const& region = reg.get<ql::region>(_region_id);
			tick const time_of_day = region.time_of_day();
			std::string time_name;
			switch (region.period_of_day()) {
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
			std::string time_string = fmt::format("Time: {} ({}, {})", region.time().value, time_of_day.value, time_name);
			sf::Text time_text{time_string, _fonts.firamono, 20};
			time_text.setColor(sf::Color::White);
			time_text.setPosition({0, 50});
			target.draw(time_text, states);
		}

		{ // Draw q- and r-axes.
			region_tile::point origin{0_span, 0_span};
			region_tile::point q_axis{5_span, 0_span};
			region_tile::point r_axis{0_span, 5_span};
			_camera.draw_lines({to_world(origin), to_world(q_axis)}, sf::Color::Green);
			_camera.draw_lines({to_world(origin), to_world(r_axis)}, sf::Color::Red);
		}
	}

	void playing::spawn_player() {
		auto player_being = umake<human>(agent::make<player>);
		_player = dynamic_cast<player*>(&player_being->agent());
		_opt_player_id = player_being->id;
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
		player_being->inventory.add(items.add(umake<gatestone>(100_mp, 100_mp, 0_tick, magic::color::green)).id);
		_region->spawn_player(std::move(player_being));
	}
}
