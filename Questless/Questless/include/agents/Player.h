//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "Agent.h"

namespace questless
{
	//! The agent representing the player's control over his or her character.
	class Player : public Agent
	{
	public:
		Player(Being& being)
			: Agent{being}
			, _world_view{nullptr}
		{}

		//! The player's current view of the world.
		WorldView const& world_view() const { return *_world_view; }

		void update_world_view() { _world_view = std::make_unique<WorldView>(being, true); }

		void act() override;

		void perceive(Effect::uptr const& effect) override;

		//! Gets a list of perceived effects and removes them from the player agent.
		//! @return All the effects the player has perceived since the last call to poll_perceived_effects().
		std::vector<Effect::uptr> poll_perceived_effects();

		// Queries and messages

		Complete send_message
			( Message::uptr message
			, std::function<Complete()> cont
			) const override;

		Complete query_count
			( CountQuery::uptr query
			, int default
			, std::optional<int> min
			, std::optional<int> max
			, std::function<Complete(std::optional<int>)> cont
			) const override;

		Complete query_magnitude
			( MagnitudeQuery::uptr query
			, double default
			, std::optional<double> min
			, std::optional<double> max
			, std::function<Complete(std::optional<double>)> cont
			) const override;

		Complete query_tile
			( TileQuery::uptr query
			, std::optional<RegionTileCoords> origin
			, std::function<bool(RegionTileCoords)> predicate
			, std::function<Complete(std::optional<RegionTileCoords>)> cont
			) const override;

		Complete query_direction
			( DirectionQuery::uptr query
			, std::function<Complete(std::optional<RegionTileCoords::Direction>)> cont
			) const override;

		Complete query_being
			( BeingQuery::uptr query
			, std::function<bool(Being&)> predicate
			, std::function<Complete(std::optional<Being*>)> cont
			) const override;

		Complete query_item
			( ItemQuery::uptr query
			, Being& source
			, std::function<bool(Being&)> predicate
			, std::function<Complete(std::optional<Item*>)> cont
			) const override;

		// Quick Time Events

		Complete get_lightning_bolt_quality(RegionTileCoords target_coords, std::function<Complete(double)> cont) const override;
	private:
		std::unique_ptr<WorldView> _world_view;

		std::vector<Effect::uptr> _perceived_effects;
	};
}
