/**
* @file    LazyAI.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the LazyAI class, which is an AI that does nothing.
*/

#ifndef LAZY_AI_H
#define LAZY_AI_H

#include "Agent.h"

namespace questless
{
	class LazyAI : public Agent
	{
	public:
		LazyAI(Being& being) : Agent{being} {}

		void act() override { being().gain_busy_time(uniform(1.0, 2.0)); }

		void perceive(const Effect::ptr&) override {}

		// Queries and messages

		Action::Complete message
			( const std::string&
			, const std::string&
			, std::function<Action::Complete()> cont
			) const override
		{
			 cont();
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
			cont(boost::none);
		}
		Action::Complete query_count
			( const std::string&
			, const std::string&
			, int
			, std::function<bool(int)>
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const override
		{
			cont(boost::none);
		}

		Action::Complete query_duration
			( const std::string&
			, const std::string&
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const override
		{
			 cont(boost::none);
		}

		Action::Complete query_magnitude
			( const std::string&
			, const std::string&
			, double
			, std::function<bool(double)>
			, std::function<Action::Complete(boost::optional<double>)> cont
			) const override
		{
			cont(boost::none);
		}

		Action::Complete query_tile
			( const std::string&
			, const std::string&
			, std::function<bool(RegionTileCoords)>
			, std::function<Action::Complete(boost::optional<RegionTileCoords>)> cont
			) const override
		{
			cont(boost::none);
		}

		Action::Complete query_being
			( const std::string&
			, const std::string&
			, std::function<bool(Being&)>
			, std::function<Action::Complete(boost::optional<Being*>)> cont
			) const override
		{
			cont(boost::none);
		}

		Action::Complete query_range
			( const std::string&
			, const std::string&
			, std::function<Action::Complete(boost::optional<int>)> cont
			) const override
		{
			cont(boost::none);
		}

		Action::Complete query_item
			( const std::string&
			, const std::string&
			, Being&
			, std::function<bool(Being&)>
			, std::function<Action::Complete(boost::optional<Item*>)> cont
			) const override
		{
			cont(boost::none);
		}
	};
}

#endif
