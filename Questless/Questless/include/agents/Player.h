/**
* @file    Player.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Player class, which is the agent responsible for the player's being.
*/

#pragma once

#include "Agent.h"

namespace questless
{
	class Player : public Agent
	{
	public:
		Player(Being& being)
			: Agent{being}
			, _world_view{nullptr}
		{}

		/// @return The player's current view of the world.
		WorldView const& world_view() const { return *_world_view; }

		void update_world_view() { _world_view = std::make_unique<WorldView>(being, true); }

		void act() override;

		void perceive(Effect::ptr const& effect) override;

		/// Gets a list of perceived effects and removes them from the player agent.
		/// @return All the effects the player has perceived since the last call to poll_effects().
		std::vector<Effect::ptr> poll_perceived_effects();

		// Queries and messages

		Action::Complete send_message
			( Message::ptr message
			, std::function<Action::Complete()> cont
			) const override;

		Action::Complete query_count
			( CountQuery::ptr query
			, int default
			, std::optional<int> min
			, std::optional<int> max
			, std::function<Action::Complete(std::optional<int>)> cont
			) const override;

		Action::Complete query_magnitude
			( MagnitudeQuery::ptr query
			, double default
			, std::optional<double> min
			, std::optional<double> max
			, std::function<Action::Complete(std::optional<double>)> cont
			) const override;

		Action::Complete query_tile
			( TileQuery::ptr query
			, std::optional<RegionTileCoords> origin
			, std::function<bool(RegionTileCoords)> predicate
			, std::function<Action::Complete(std::optional<RegionTileCoords>)> cont
			) const override;

		Action::Complete query_being
			( BeingQuery::ptr query
			, std::function<bool(Being&)> predicate
			, std::function<Action::Complete(std::optional<Being*>)> cont
			) const override;

		Action::Complete query_item
			( ItemQuery::ptr query
			, Being& source
			, std::function<bool(Being&)> predicate
			, std::function<Action::Complete(std::optional<Item*>)> cont
			) const override;

		// Quick Time Events

		Action::Complete get_lightning_bolt_quality(RegionTileCoords target_coords, std::function<Action::Complete(double)> cont) const override;
	private:
		std::unique_ptr<WorldView> _world_view;

		std::vector<Effect::ptr> _perceived_effects;
	};
}
