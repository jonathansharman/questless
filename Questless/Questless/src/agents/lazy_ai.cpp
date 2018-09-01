//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "lazy_ai.hpp"

#include "queries/all_queries.hpp"

#include "game.hpp"
#include "utility/random.hpp"

namespace ql {
	complete lazy_ai::act() { being.busy_time += uniform(1.0, 2.0); }

	complete lazy_ai::send_message
		( uptr<message>
		, std::function<complete()> cont
		) const
	{
		return cont();
	}

	//complete lazy_ai::query_count
	//	( uptr<count_query>
	//	, int
	//	, std::optional<int>
	//	, std::optional<int>
	//	, std::function<complete(std::optional<int>)> cont
	//	) const
	//{
	//	return cont(std::nullopt);
	//}

	complete lazy_ai::query_magnitude
		( uptr<magnitude_query>
		, double
		, std::optional<double>
		, std::optional<double>
		, std::function<complete(std::optional<double>)> cont
		) const
	{
		return cont(std::nullopt);
	}

	complete lazy_ai::query_tile
		( uptr<tile_query>
		, std::optional<region_tile::point>
		, std::function<bool(region_tile::point)>
		, std::function<complete(std::optional<region_tile::point>)> cont
		) const
	{
		return cont(std::nullopt);
	}

	//complete lazy_ai::query_direction
	//	( uptr<direction_query>
	//	, std::function<complete(std::optional<region_tile::direction>)> cont
	//	) const
	//{
	//	return cont(std::nullopt);
	//}

	complete lazy_ai::query_vector
		( uptr<vector_query>
		, std::optional<region_tile::point>
		, std::function<bool(region_tile::vector)>
		, std::function<complete(std::optional<region_tile::vector>)> cont
		) const
	{
		return cont(std::nullopt);
	}

	complete lazy_ai::query_being
		( uptr<being_query>
		, std::function<bool(ql::being&)>
		, std::function<complete(std::optional<ql::being*>)> cont
		) const
	{
		return cont(std::nullopt);
	}

	//complete lazy_ai::query_item
	//	( uptr<item_query>
	//	, ql::being&
	//	, std::function<bool(ql::being&)>
	//	, std::function<complete(std::optional<item*>)> cont
	//	) const
	//{
	//	return cont(std::nullopt);
	//}
}
