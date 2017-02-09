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

		Action::Complete message
			( std::string const& title
			, std::string const& prompt
			, std::function<Action::Complete()> cont
			) const override;

		Action::Complete query_count
			( std::string const& title
			, std::string const& prompt
			, int default
			, boost::optional<int> min
			, boost::optional<int> max
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const override;

		Action::Complete query_magnitude
			( std::string const& title
			, std::string const& prompt
			, double default
			, boost::optional<double> min
			, boost::optional<double> max
			, std::function<Action::Complete(boost::optional<double>)> cont
			) const override;

		Action::Complete query_tile
			( std::string const& title
			, std::string const& prompt
			, boost::optional<RegionTileCoords> origin
			, std::function<bool(RegionTileCoords)> predicate
			, std::function<Action::Complete(boost::optional<RegionTileCoords>)> cont
			) const override;

		Action::Complete query_being
			( std::string const& title
			, std::string const& prompt
			, std::function<bool(Being&)> predicate
			, std::function<Action::Complete(boost::optional<Being*>)> cont
			) const override;

		Action::Complete query_range
			( std::string const& title
			, std::string const& prompt
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const override;

		Action::Complete query_item
			( std::string const& title
			, std::string const& prompt
			, Being& source
			, std::function<bool(Being&)> predicate
			, std::function<Action::Complete(boost::optional<Item*>)> cont
			) const override;

		virtual Action::Complete query_list
			( units::ScreenPoint origin
			, std::string title
			, std::vector<std::string> options
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const override;

		// Quick Time Events

		Action::Complete get_lightning_bolt_quality(units::GamePoint target, std::function<Action::Complete(double)> cont) override;
	private:
		std::unique_ptr<WorldView> _world_view;

		std::vector<Effect::ptr> _perceived_effects;
	};
}
