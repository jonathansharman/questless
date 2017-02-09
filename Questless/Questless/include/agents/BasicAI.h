/**
* @file    BasicAI.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the BasicAI class, an AI suitable for most NPCs.
*/

#pragma once

#include "Agent.h"

namespace questless
{
	class BasicAI : public Agent
	{
	public:
		BasicAI(Being& being) : Agent{being} {}

		void act() override;

		void perceive(Effect::ptr const&) override {}

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

		Action::Complete query_list
			( units::ScreenPoint
			, std::string
			, std::vector<std::string>
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const override;
	private:
		enum class State { move, idle };
		State _state = State::idle;
	};
}
