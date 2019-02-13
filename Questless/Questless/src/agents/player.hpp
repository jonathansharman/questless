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
		player(sf::Window const& window, rsrc::fonts const& fonts, ql::being& being);

		~player();

		//! The player's current view of the world.
		world_view const& world_view() const;

		//! Updates the player's world view based on its internal being.
		void update_view();

		complete act() final;

		void perceive(effects::effect const& effect) final;

		//! Gets a list of perceived effects and removes them from the player agent.
		//! @return All the effects the player has perceived since the last call to poll_perceived_effects().
		std::vector<effects::effect> poll_perceived_effects();

		// Queries and messages

		complete send_message(queries::message::query message, std::function<complete()> cont) final;

		complete query_count(queries::count::query query,
			int default_value,
			std::optional<int> min,
			std::optional<int> max,
			std::function<complete(std::optional<int>)> cont) final;

		complete query_magnitude(queries::magnitude::query query,
			double default_value,
			std::optional<double> min,
			std::optional<double> max,
			std::function<complete(std::optional<double>)> cont) final;

		complete query_tile(queries::tile::query query,
			std::optional<region_tile::point> origin,
			std::function<bool(region_tile::point)> predicate,
			std::function<complete(std::optional<region_tile::point>)> cont) final;

		complete query_direction(queries::direction::query query,
			std::function<complete(std::optional<region_tile::direction>)> cont) final;

		complete query_vector(queries::vector::query query,
			std::optional<region_tile::point> origin,
			std::function<bool(region_tile::vector)> predicate,
			std::function<complete(std::optional<region_tile::vector>)> cont) final;

		complete query_being(queries::being::query query,
			std::function<bool(ql::being&)> predicate,
			std::function<complete(std::optional<ql::being*>)> cont) final;

		complete query_item(queries::item::query query,
			ql::being& source,
			std::function<bool(ql::being&)> predicate,
			std::function<complete(std::optional<item*>)> cont) final;

		// Quick Time Events

		complete aim_missile(region_tile::point source_coords,
			ql::being& target_being,
			std::function<complete(body_part*)> cont) final;
		complete get_shock_quality(region_tile::point target_coords, std::function<complete(double)> cont) final;

		complete incant(gatestone& gatestone, std::function<complete(std::optional<magic::spell> const&)> cont) final;

	private:
		sf::Window const& _window;
		rsrc::fonts const& _fonts;

		uptr<ql::world_view> _world_view = nullptr;

		uptr<player_action_dialog> _player_action_dialog;
		std::deque<uptr<dialog>> _dialogs;

		ql::hud _hud;

		std::vector<effects::effect> _perceived_effects;

		complete query_player_choice(std::function<void(player_action_dialog::choice)> cont);
	};
}
