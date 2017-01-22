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

		void act() override { being().busy_time += uniform(1.0, 2.0); }

		void perceive(const Effect::ptr&) override {}

		// Queries and messages

		Action::Complete message
			( const std::string&
			, const std::string&
			, std::function<Action::Complete()> cont
			) const override
		{
			return cont();
		}

		Action::Complete query_count
			( const std::string&
			, const std::string&
			, int
			, boost::optional<int>
			, boost::optional<int>
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const override
		{
			return cont(boost::none);
		}
		Action::Complete query_count
			( const std::string&
			, const std::string&
			, int
			, std::function<bool(int)>
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const override
		{
			return cont(boost::none);
		}

		Action::Complete query_duration
			( const std::string&
			, const std::string&
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const override
		{
			return cont(boost::none);
		}

		Action::Complete query_magnitude
			( const std::string&
			, const std::string&
			, double
			, std::function<bool(double)>
			, std::function<Action::Complete(boost::optional<double>)> cont
			) const override
		{
			return cont(boost::none);
		}

		Action::Complete query_tile
			( const std::string&
			, const std::string&
			, boost::optional<RegionTileCoords>
			, std::function<bool(RegionTileCoords)>
			, std::function<Action::Complete(boost::optional<RegionTileCoords>)> cont
			) const override
		{
			return cont(boost::none);
		}

		Action::Complete query_being
			( const std::string&
			, const std::string&
			, std::function<bool(Being&)>
			, std::function<Action::Complete(boost::optional<Being*>)> cont
			) const override
		{
			return cont(boost::none);
		}

		Action::Complete query_range
			( const std::string&
			, const std::string&
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const override
		{
			return cont(boost::none);
		}

		Action::Complete query_item
			( const std::string&
			, const std::string&
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
