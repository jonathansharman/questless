//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "agent.h"

namespace ql
{
	//! An AI that does nothing.
	//!
	//! Lazy AI respond to all queries with nullopt and simply idle when it's their turn.
	class lazy_ai : public agent
	{
	public:
		lazy_ai(ql::being& being) : agent{being} {}

		void act() final;

		void perceive(sptr<effect> const&) final {}

		// Queries and messages

		complete send_message
			( uptr<message>
			, std::function<complete()> cont
			) const final
		{
			return cont();
		}

		complete query_count
			( uptr<count_query>
			, int
			, std::optional<int>
			, std::optional<int>
			, std::function<complete(std::optional<int>)> cont
			) const final
		{
			return cont(std::nullopt);
		}

		complete query_magnitude
			( uptr<magnitude_query>
			, double
			, std::optional<double>
			, std::optional<double>
			, std::function<complete(std::optional<double>)> cont
			) const final
		{
			return cont(std::nullopt);
		}

		complete query_tile
			( uptr<tile_query>
			, std::optional<region_tile::point>
			, std::function<bool(region_tile::point)>
			, std::function<complete(std::optional<region_tile::point>)> cont
			) const final
		{
			return cont(std::nullopt);
		}

		complete query_direction
			( uptr<direction_query>
			, std::function<complete(std::optional<region_tile::direction>)> cont
			) const final
		{
			return cont(std::nullopt);
		}

		complete query_vector
			( uptr<vector_query>
			, std::optional<region_tile::point>
			, std::function<bool(region_tile::vector)>
			, std::function<complete(std::optional<region_tile::vector>)> cont
			) const final
		{
			return cont(std::nullopt);
		}

		complete query_being
			( uptr<being_query>
			, std::function<bool(ql::being&)>
			, std::function<complete(std::optional<ql::being*>)> cont
			) const final
		{
			return cont(std::nullopt);
		}

		complete query_item
			( uptr<item_query>
			, ql::being&
			, std::function<bool(ql::being&)>
			, std::function<complete(std::optional<item*>)> cont
			) const final
		{
			return cont(std::nullopt);
		}
	};
}
