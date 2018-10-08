//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "lazy_ai.hpp"

#include "game.hpp"
#include "utility/random.hpp"

namespace ql {
	complete lazy_ai::act() {
		being.busy_time += tick{uniform(1, 3)};
		return complete{};
	}

	complete lazy_ai::send_message
		( queries::message::any
		, std::function<complete()> cont
		) const
	{
		return cont();
	}

	complete lazy_ai::query_count
		( queries::count::any
		, int
		, std::optional<int>
		, std::optional<int>
		, std::function<complete(std::optional<int>)> cont
		) const
	{
		return cont(std::nullopt);
	}

	complete lazy_ai::query_magnitude
		( queries::magnitude::any
		, double
		, std::optional<double>
		, std::optional<double>
		, std::function<complete(std::optional<double>)> cont
		) const
	{
		return cont(std::nullopt);
	}

	complete lazy_ai::query_tile
		( queries::tile::any
		, std::optional<region_tile::point>
		, std::function<bool(region_tile::point)>
		, std::function<complete(std::optional<region_tile::point>)> cont
		) const
	{
		return cont(std::nullopt);
	}

	//complete lazy_ai::query_direction
	//	( queries::direction::any
	//	, std::function<complete(std::optional<region_tile::direction>)> cont
	//	) const
	//{
	//	return cont(std::nullopt);
	//}

	complete lazy_ai::query_vector
		( queries::vector::any
		, std::optional<region_tile::point>
		, std::function<bool(region_tile::vector)>
		, std::function<complete(std::optional<region_tile::vector>)> cont
		) const
	{
		return cont(std::nullopt);
	}

	complete lazy_ai::query_being
		( queries::being::any
		, std::function<bool(ql::being&)>
		, std::function<complete(std::optional<ql::being*>)> cont
		) const
	{
		return cont(std::nullopt);
	}

	//complete lazy_ai::query_item
	//	( queries::item::any
	//	, ql::being&
	//	, std::function<bool(ql::being&)>
	//	, std::function<complete(std::optional<item*>)> cont
	//	) const
	//{
	//	return cont(std::nullopt);
	//}
}
