//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

#include "view_space.hpp"

#include "reg.hpp"
#include "utility/reference.hpp"

#include <array>
#include <future>
#include <optional>
#include <vector>

namespace ql {
	namespace effects {
		struct effect;
	}
	namespace rsrc {
		struct fonts;
	}

	//! Head-up display, the root element for interfacing with the player.
	struct hud : widget {
		//! @param window The game window.
		//! @param fonts The game fonts.
		//! @param player_id The ID of the player-controlled being.
		hud(widget& parent, sf::Window& window, rsrc::fonts const& fonts, ent player_id)
			: widget{&parent}, _window{window}, _fonts{fonts}, _player_id{player_id} {}

		view::vector get_local_offset() const final;

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
		static constexpr size_t _hotbar_size = 10;

		sf::Window& _window;
		rsrc::fonts const& _fonts;

		ent _player_id;
		std::array<std::optional<ent>, _hotbar_size> _hotbar;
		int _last_used_hotbar_idx = 0;
		std::promise<void> _pass_promise;

		void update_displayed_items(ent player_id);

		std::vector<std::tuple<sf::String, std::function<void()>>> get_item_options(ent item_id);
	};
}
