//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "playing.hpp"

#include "rsrc/fonts.hpp"
#include "utility/reference.hpp"
#include "world/region.hpp"
#include "world/spawn_player.hpp"
#include "world/world_space.hpp"

#include <fmt/format.h>

using namespace vecx;
using namespace vecx::literals;

namespace ql::scenes {
	playing::playing(sf::RenderWindow& window, rsrc::fonts const& fonts, ent region_id)
		: scene{window, fonts}
		, _region_id{region_id}
		, _player_id{spawn_player(_region_id, &_hud)}
		, _layout{window}
		, _hud{_layout, window, fonts, _player_id} //
	{}

	update_result playing::scene_subupdate(sec elapsed_time, std::vector<sf::Event>& events) {
		// Update HUD.
		_hud.update(elapsed_time, events);

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
			_view.zoom(1.0f + im.scroll() / 10.0f);

			// Rotate camera.
			degrees angle = degrees{_view.getRotation()};
			if (im.down(sf::Keyboard::Numpad9)) {
				_view.rotate(static_cast<float>((-3.0_deg - angle / 20.0).value));
			} else if (im.down(sf::Keyboard::Numpad7)) {
				_view.rotate(static_cast<float>((3.0_deg - angle / 20.0).value));
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
			time_text.setFillColor(sf::Color::White);
			time_text.setPosition({0, 50});
			target.draw(time_text, states);
		}
	}
}
