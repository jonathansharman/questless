//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "lazy_ai.hpp"

#include "entities/beings/being.hpp"
#include "game.hpp"
#include "utility/random.hpp"

namespace ql {
	complete lazy_ai::act() {
		being.cond.busy_time += tick{uniform(1, 3)};
		return complete{};
	}

	complete lazy_ai::send_message(queries::message::query, std::function<complete()> cont) {
		return cont();
	}

	complete lazy_ai::query_count(queries::count::query,
		int,
		std::optional<int>,
		std::optional<int>,
		std::function<complete(std::optional<int>)> cont) {
		return cont(std::nullopt);
	}

	complete lazy_ai::query_magnitude(queries::magnitude::query,
		double,
		std::optional<double>,
		std::optional<double>,
		std::function<complete(std::optional<double>)> cont) {
		return cont(std::nullopt);
	}

	complete lazy_ai::query_tile(queries::tile::query,
		std::optional<region_tile::point>,
		std::function<bool(region_tile::point)>,
		std::function<complete(std::optional<region_tile::point>)> cont) {
		return cont(std::nullopt);
	}

	complete lazy_ai::query_direction(queries::direction::query,
		std::function<complete(std::optional<region_tile::direction>)> cont) {
		return cont(std::nullopt);
	}

	complete lazy_ai::query_vector(queries::vector::query,
		std::optional<region_tile::point>,
		std::function<bool(region_tile::vector)>,
		std::function<complete(std::optional<region_tile::vector>)> cont) {
		return cont(std::nullopt);
	}

	complete lazy_ai::query_being(queries::being::query,
		std::function<bool(ql::being&)>,
		std::function<complete(std::optional<ql::being*>)> cont) {
		return cont(std::nullopt);
	}

	complete lazy_ai::query_item(queries::item::query,
		ql::being&,
		std::function<bool(ql::being&)>,
		std::function<complete(std::optional<item*>)> cont) {
		return cont(std::nullopt);
	}
}
