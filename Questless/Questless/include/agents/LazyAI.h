//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "Agent.h"

namespace questless
{
	//! An AI that does nothing.
	//!
	//! Lazy AI respond to all queries with nullopt and simply idle when it's their turn.
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
