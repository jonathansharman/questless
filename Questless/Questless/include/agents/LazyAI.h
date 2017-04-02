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

		void perceive(Effect::uptr const&) override {}

		// Queries and messages

		Complete send_message
			( Message::uptr
			, std::function<Complete()> cont
			) const override
		{
			return cont();
		}

		Complete query_count
			( CountQuery::uptr
			, int
			, std::optional<int>
			, std::optional<int>
			, std::function<Complete(std::optional<int>)> cont
			) const override
		{
			return cont(std::nullopt);
		}

		Complete query_magnitude
			( MagnitudeQuery::uptr
			, double
			, std::optional<double>
			, std::optional<double>
			, std::function<Complete(std::optional<double>)> cont
			) const override
		{
			return cont(std::nullopt);
		}

		Complete query_tile
			( TileQuery::uptr
			, std::optional<RegionTileCoords>
			, std::function<bool(RegionTileCoords)>
			, std::function<Complete(std::optional<RegionTileCoords>)> cont
			) const override
		{
			return cont(std::nullopt);
		}

		Complete query_direction
			( DirectionQuery::uptr
			, std::function<Complete(std::optional<RegionTileCoords::Direction>)> cont
			) const override
		{
			return cont(std::nullopt);
		}

		Complete query_being
			( BeingQuery::uptr
			, std::function<bool(Being&)>
			, std::function<Complete(std::optional<Being*>)> cont
			) const override
		{
			return cont(std::nullopt);
		}

		Complete query_item
			( ItemQuery::uptr
			, Being&
			, std::function<bool(Being&)>
			, std::function<Complete(std::optional<Item*>)> cont
			) const override
		{
			return cont(std::nullopt);
		}
	};
}
