//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "main_menu.hpp"

#include "hud.hpp"

#include "agents/agent.hpp"
#include "world/region.hpp"
#include "world/spawn_player.hpp"

namespace ql {
	main_menu::main_menu(uptr<widget>& root, rsrc::fonts const& fonts) : _root{root} {
		//! @todo Implement main menu stuff instead of just immediately switching into the game.

		// Create the main region.
		id region_id = make_region(reg.create(), "Region 1");
		// Spawn the player into the main region.
		auto player_id = create_and_spawn_player(region_id);
		// Create HUD.
		_hud = umake<hud>(root, fonts, region_id, player_id);
	}

	main_menu::~main_menu() = default;

	auto main_menu::get_size() const -> view::vector {
		return _size;
	}

	auto main_menu::update(sec) -> void {
		// Initialize HUD size and position.
		_hud->on_parent_resize(_size);
		_hud->set_position(_position);
		// Switch to the HUD.
		_root = std::move(_hud);
	}

	auto main_menu::draw(sf::RenderTarget&, sf::RenderStates) const -> void {}

	auto main_menu::set_position(view::point position) -> void {
		_position = position;
	}

	auto main_menu::get_position() const -> view::point {
		return _position;
	}

	auto main_menu::on_parent_resize(view::vector parent_size) -> void {
		_size = parent_size;
	}

	auto main_menu::on_request_quit() -> event_handled {
		_root = nullptr;
		return event_handled::yes;
	}
}
