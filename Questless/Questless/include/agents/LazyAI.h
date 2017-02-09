/**
* @file    LazyAI.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the LazyAI class, which is an AI that does nothing.
*/

#pragma once

#include "Agent.h"

namespace questless
{
	class LazyAI : public Agent
	{
	public:
		LazyAI(Being& being) : Agent{being} {}

		void act() override { being.busy_time += uniform(1.0, 2.0); }

		void perceive(Effect::ptr const&) override {}

		// Queries and messages

		Action::Complete message
			( std::string const&
			, std::string const&
			, std::function<Action::Complete()> cont
			) const override
		{
			return cont();
		}

		Action::Complete query_count
			( std::string const&
			, std::string const&
			, int
			, boost::optional<int>
			, boost::optional<int>
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const override
		{
			return cont(boost::none);
		}

		Action::Complete query_magnitude
			( std::string const&
			, std::string const&
			, double
			, boost::optional<double>
			, boost::optional<double>
			, std::function<Action::Complete(boost::optional<double>)> cont
			) const override
		{
			return cont(boost::none);
		}

		Action::Complete query_tile
			( std::string const&
			, std::string const&
			, boost::optional<RegionTileCoords>
			, std::function<bool(RegionTileCoords)>
			, std::function<Action::Complete(boost::optional<RegionTileCoords>)> cont
			) const override
		{
			return cont(boost::none);
		}

		Action::Complete query_being
			( std::string const&
			, std::string const&
			, std::function<bool(Being&)>
			, std::function<Action::Complete(boost::optional<Being*>)> cont
			) const override
		{
			return cont(boost::none);
		}

		Action::Complete query_range
			( std::string const&
			, std::string const&
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const override
		{
			return cont(boost::none);
		}

		Action::Complete query_item
			( std::string const&
			, std::string const&
			, Being&
			, std::function<bool(Being&)>
			, std::function<Action::Complete(boost::optional<Item*>)> cont
			) const override
		{
			return cont(boost::none);
		}

		Action::Complete query_list
			( units::ScreenPoint
			, std::string
			, std::vector<std::string>
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const override
		{
			return cont(boost::none);
		}
	};
}
