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

		void act(Game&) override { _being.gain_busy_time(uniform(1.0, 2.0)); }

		// Queries and messages

		 void message
			( Game&
			, const std::string&
			, const std::string&
			, std::function<void()> cont
			) const override
		 {
			 cont();
		 }

		 void query_count
			( Game&
			, const std::string&
			, const std::string&
			, int
			, optional<int>
			, optional<int>
			, std::function<void(optional<int>)> cont
			) const override
		 {
			 cont(nullopt);
		 }
		 void query_count
			( Game&
			, const std::string&
			, const std::string&
			, int
			, std::function<bool(int)>
			, std::function<void(optional<int>)> cont
			) const override
		 {
			 cont(nullopt);
		 }

		 void query_duration
			( Game&
			, const std::string&
			, const std::string&
			, std::function<void(optional<int>)> cont
			) const override
		 {
			 cont(nullopt);
		 }

		 void query_magnitude
			( Game&
			, const std::string&
			, const std::string&
			, double
			, std::function<bool(double)>
			, std::function<void(optional<double>)> cont
			) const override
		 {
			 cont(nullopt);
		 }

		 void query_tile
			( Game&
			, const std::string&
			, const std::string&
			, std::function<bool(RegionTileCoords)>
			, std::function<void(optional<RegionTileCoords>)> cont
			) const override
		 {
			 cont(nullopt);
		 }

		 void query_being
			( Game&
			, const std::string&
			, const std::string&
			, std::function<bool(Being&)>
			, std::function<void(optional<Being*>)> cont
			) const override
		 {
			 cont(nullopt);
		 }

		 void query_range
			( Game&
			, const std::string&
			, const std::string&
			, std::function<void(optional<int>)> cont
			) const override
		 {
			 cont(nullopt);
		 }

		 void query_item
			( Game&
			, const std::string&
			, const std::string&
			, const Being&
			, std::function<bool(Being&)>
			, std::function<void(optional<Item*>)> cont
			) const override
		 {
			 cont(nullopt);
		 }
	};
}

#endif
