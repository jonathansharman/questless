//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <optional>
#include <variant>

#include "dialog.hpp"
#include "world/coordinates.hpp"

namespace ql {
	struct hud;

	//! Dialog for the player to choose an action for his or her controlled being.
	struct player_action_dialog : dialog {
		struct idle {
			bool prolonged;
		};
		struct move {
			bool strafe;
			region_tile::direction direction;
		};
		struct use {
			std::size_t index;
		};
		using choice = std::variant<idle, move, use>;

		player_action_dialog(sf::Window const& window, rsrc::fonts const& fonts, hud& hud, std::function<void(choice)> cont)
			: dialog{window, fonts}, _hud{hud}, _cont{std::move(cont)} {}

		void update(sec elapsed_time, input_manager& im) final;

		void draw(sf::RenderTarget&, sf::RenderStates) const final {}

	private:
		hud& _hud;
		continuation<choice> _cont;
		static std::size_t _last_used;
	};
}
