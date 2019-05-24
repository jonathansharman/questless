//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "agent.hpp"

#include "effects/effect.hpp"
#include "ui/dialog.hpp"
#include "ui/hud.hpp"
#include "ui/player_action_dialog.hpp"

#include <deque>

namespace ql {
	namespace rsrc {
		struct fonts;
	}
	struct world_view;

	//! The agent representing the player's control over his or her character.
	struct player : agent {
		//! @param window The window to use when spawning dialogs for the player.
		//! @param being The being this player controls.
		player(ent id, sf::Window const& window, rsrc::fonts const& fonts);

		~player();

		//! The player's current view of the world.
		world_view const& world_view() const;

		//! Updates the player's world view based on its internal being.
		void update_view();

		action::result act() final;

		void perceive(effects::effect const& effect) final;

		//! Gets a list of perceived effects and removes them from the player agent.
		//! @return All the effects the player has perceived since the last call to poll_perceived_effects().
		std::vector<effects::effect> poll_perceived_effects();

	private:
		sf::Window const& _window;
		rsrc::fonts const& _fonts;

		uptr<ql::world_view> _world_view = nullptr;

		uptr<player_action_dialog> _player_action_dialog;
		std::deque<uptr<dialog>> _dialogs;

		ql::hud _hud;

		std::vector<effects::effect> _perceived_effects;

		std::vector<std::tuple<sf::String>, std::function<void()>> get_item_options(agent& agent, ent item_id);

		complete query_player_choice(std::function<void(player_action_dialog::choice)> cont);
	};
}
