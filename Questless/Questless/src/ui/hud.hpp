//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

#include "hotbar.hpp"
#include "inventory_widget.hpp"
#include "panel.hpp"
#include "view_space.hpp"
#include "window_widget.hpp"
#include "world_widget.hpp"

#include "reg.hpp"

#include <future>

namespace ql {
	namespace effects {
		struct effect;
	}
	namespace rsrc {
		struct fonts;
		struct hud;
	}
	struct inventory_widget;

	//! Head-up display, the root element for interfacing with the player.
	struct hud : widget {
		//! @param player_id The ID of the player-controlled being.
		hud( //
			widget& parent,
			rsrc::hud const& resources,
			ent player_id);

		view::vector get_size() const final;

		void update(sec elapsed_time, std::vector<sf::Event>& events) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

		//! Renders @p effect for the player.
		void render_effect(effects::effect const& effect);

		//! Gets a future which is set after the player passes the current turn.
		std::future<void> pass_future() {
			return _pass_promise.get_future();
		}

	private:
		ent _player_id;
		std::promise<void> _pass_promise;

		rsrc::hud const& _resources;
		panel _panel;
		world_widget _world_widget;
		hotbar _hotbar;
		inventory_widget _inv;
		bool _show_inv = false;

		std::vector<std::tuple<sf::String, std::function<void()>>> get_item_options(ent item_id);

		void pass();
	};
}
