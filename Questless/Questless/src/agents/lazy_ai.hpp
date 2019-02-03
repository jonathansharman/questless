//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "agent.hpp"

namespace ql {
	//! An AI that does nothing.
	//!
	//! Lazy AI respond to all queries with nullopt and simply idle when it's their turn.
	struct lazy_ai : agent {
		lazy_ai(ql::being& being) : agent{being} {}

		complete act() final;

		void perceive(effects::effect const&) final {}

		// Queries and messages

		complete send_message(queries::message::query, std::function<complete()> cont) final;

		complete query_count(queries::count::query,
			int,
			std::optional<int>,
			std::optional<int>,
			std::function<complete(std::optional<int>)> cont) final;

		complete query_magnitude(queries::magnitude::query,
			double,
			std::optional<double>,
			std::optional<double>,
			std::function<complete(std::optional<double>)> cont) final;

		complete query_tile(queries::tile::query,
			std::optional<region_tile::point>,
			std::function<bool(region_tile::point)>,
			std::function<complete(std::optional<region_tile::point>)> cont) final;

		complete query_direction(queries::direction::query,
			std::function<complete(std::optional<region_tile::direction>)> cont) final;

		complete query_vector(queries::vector::query,
			std::optional<region_tile::point>,
			std::function<bool(region_tile::vector)>,
			std::function<complete(std::optional<region_tile::vector>)> cont) final;

		complete query_being(queries::being::query,
			std::function<bool(ql::being&)>,
			std::function<complete(std::optional<ql::being*>)> cont) final;

		complete query_item(queries::item::query,
			ql::being&,
			std::function<bool(ql::being&)>,
			std::function<complete(std::optional<item*>)> cont) final;
	};
}
