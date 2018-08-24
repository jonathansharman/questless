//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor_pattern.hpp"

namespace ql {
	struct tile_query_ranged_attack_target;
	struct tile_query_shock_target;
	struct tile_query_teleport_target;

	using tile_query_subtype_list = type_list::of_t
		< tile_query_ranged_attack_target
		, tile_query_shock_target
		, tile_query_teleport_target
		>;

	DEFINE_VISITORS(tile_query, tile_query_subtype_list)

	//! A request to an agent for tile coordinates.
	struct tile_query : public element<tile_query_subtype_list> {
		virtual ~tile_query() = default;
	};

	DEFINE_ELEMENT_BASE(tile_query, tile_query)

	struct tile_query_ranged_attack_target : tile_query_base<tile_query_ranged_attack_target> {
		int range;
		tile_query_ranged_attack_target(int range) : range{ range } {}
	};
	struct tile_query_shock_target : tile_query_base<tile_query_shock_target> {};
	struct tile_query_teleport_target : tile_query_base<tile_query_teleport_target> {};
}
