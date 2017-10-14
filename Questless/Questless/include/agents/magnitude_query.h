//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor_pattern.h"

namespace ql
{
	struct magnitude_query_heal;
	struct magnitude_query_shock;
	struct magnitude_query_wait_time;

	using magnitude_query_subtype_list = type_list::of_t
		< magnitude_query_heal
		, magnitude_query_shock
		, magnitude_query_wait_time
		>;

	DEFINE_VISITORS(magnitude_query, magnitude_query_subtype_list)

	//! A request to an agent for a magnitude.
	struct magnitude_query : element<magnitude_query_subtype_list>
	{
		virtual ~magnitude_query() = default;
	};

	DEFINE_ELEMENT_BASE(magnitude_query, magnitude_query)

	struct magnitude_query_heal : magnitude_query_base<magnitude_query_heal> {};
	struct magnitude_query_shock : magnitude_query_base<magnitude_query_shock> {};
	struct magnitude_query_wait_time : magnitude_query_base<magnitude_query_wait_time> {};
}
