/**
* @file    Player.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Player class, which is the agent responsible for the player's being.
*/

#ifndef PLAYER_H
#define PLAYER_H

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
		const WorldView& world_view() const { return *_world_view; }

		void update_world_view() { _world_view = std::make_unique<WorldView>(being(), true); }

		void act() override;

		void perceive(const Effect::ptr& effect) override;

		/// Gets a list of perceived effects and removes them from the player agent.
		/// @return All the effects the player has perceived since the last call to poll_effects().
		std::vector<Effect::ptr> poll_perceived_effects();

		// Queries and messages

		Action::Complete message
			( const std::string& title
			, const std::string& prompt
			, std::function<Action::Complete()> cont
			) const override;

		Action::Complete query_count
			( const std::string& title
			, const std::string& prompt
			, int default
			, boost::optional<int> min
			, boost::optional<int> max
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const override;
		Action::Complete query_count
			( const std::string& title
			, const std::string& prompt
			, int default
			, std::function<bool(int)> predicate
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const override;

		Action::Complete query_duration
			( const std::string& title
			, const std::string& prompt
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const override;

		Action::Complete query_magnitude
			( const std::string& title
			, const std::string& prompt
			, double default
			, std::function<bool(double)> predicate
			, std::function<Action::Complete(boost::optional<double>)> cont
			) const override;

		Action::Complete query_tile
			( const std::string& title
			, const std::string& prompt
			, std::function<bool(RegionTileCoords)> predicate
			, std::function<Action::Complete(boost::optional<RegionTileCoords>)> cont
			) const override;

		Action::Complete query_being
			( const std::string& title
			, const std::string& prompt
			, std::function<bool(Being&)> predicate
			, std::function<Action::Complete(boost::optional<Being*>)> cont
			) const override;

		Action::Complete query_range
			( const std::string& title
			, const std::string& prompt
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const override;

		Action::Complete query_item
			( const std::string& title
			, const std::string& prompt
			, Being& source
			, std::function<bool(Being&)> predicate
			, std::function<Action::Complete(boost::optional<Item*>)> cont
			) const override;

		virtual Action::Complete query_list
			( sdl::Point origin
			, std::string title
			, std::vector<std::string> options
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const override;
	private:
		std::unique_ptr<WorldView> _world_view;

		std::vector<Effect::ptr> _perceived_effects;
	};
}

#endif