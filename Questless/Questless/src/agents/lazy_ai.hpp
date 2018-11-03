//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "agent.hpp"

namespace ql {
	//! An AI that does nothing.
	//!
	//! Lazy AI respond to all queries with nullopt and simply idle when it's their turn.
	class lazy_ai : public agent {
	public:
		lazy_ai(ql::being& being) : agent{being} {}

		complete act() final;

		void perceive(sptr<effect> const&) final {}

		// Queries and messages

		complete send_message
			( queries::message::any
			, std::function<complete()> cont
			) final;

		complete query_count
			( queries::count::any
			, int
			, std::optional<int>
			, std::optional<int>
			, std::function<complete(std::optional<int>)> cont
			) final;

		complete query_magnitude
			( queries::magnitude::any
			, double
			, std::optional<double>
			, std::optional<double>
			, std::function<complete(std::optional<double>)> cont
			) final;

		complete query_tile
			( queries::tile::any
			, std::optional<region_tile::point>
			, std::function<bool(region_tile::point)>
			, std::function<complete(std::optional<region_tile::point>)> cont
			) final;

		complete query_direction
			( queries::direction::any
			, std::function<complete(std::optional<region_tile::direction>)> cont
			) final;

		complete query_vector
			( queries::vector::any
			, std::optional<region_tile::point>
			, std::function<bool(region_tile::vector)>
			, std::function<complete(std::optional<region_tile::vector>)> cont
			) final;

		complete query_being
			( queries::being::any
			, std::function<bool(ql::being&)>
			, std::function<complete(std::optional<ql::being*>)> cont
			) final;

		complete query_item
			( queries::item::any
			, ql::being&
			, std::function<bool(ql::being&)>
			, std::function<complete(std::optional<item*>)> cont
			) final;
	};
}
