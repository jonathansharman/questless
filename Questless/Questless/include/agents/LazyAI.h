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

		Action::Complete send_message
			( Message::ptr
			, std::function<Action::Complete()> cont
			) const override
		{
			return cont();
		}

		Action::Complete query_count
			( CountQuery::ptr
			, int
			, std::optional<int>
			, std::optional<int>
			, std::function<Action::Complete(std::optional<int>)> cont
			) const override
		{
			return cont(std::nullopt);
		}

		Action::Complete query_magnitude
			( MagnitudeQuery::ptr
			, double
			, std::optional<double>
			, std::optional<double>
			, std::function<Action::Complete(std::optional<double>)> cont
			) const override
		{
			return cont(std::nullopt);
		}

		Action::Complete query_tile
			( TileQuery::ptr
			, std::optional<RegionTileCoords>
			, std::function<bool(RegionTileCoords)>
			, std::function<Action::Complete(std::optional<RegionTileCoords>)> cont
			) const override
		{
			return cont(std::nullopt);
		}

		Action::Complete query_being
			( BeingQuery::ptr
			, std::function<bool(Being&)>
			, std::function<Action::Complete(std::optional<Being*>)> cont
			) const override
		{
			return cont(std::nullopt);
		}

		Action::Complete query_item
			( ItemQuery::ptr
			, Being&
			, std::function<bool(Being&)>
			, std::function<Action::Complete(std::optional<Item*>)> cont
			) const override
		{
			return cont(std::nullopt);
		}
	};
}
