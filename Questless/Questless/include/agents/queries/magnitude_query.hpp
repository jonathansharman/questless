//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor_pattern.hpp"

namespace ql
{
	#define MAGNITUDE_QUERY_FIRST \
		X(magnitude_query_heal)	
	#define MAGNITUDE_QUERY_REST \
		X(magnitude_query_shock) \
		X(magnitude_query_wait_time)

	#define X(subtype) struct subtype;
	MAGNITUDE_QUERY_FIRST
	MAGNITUDE_QUERY_REST
	#undef X

	using magnitude_query_subtype_list = type_list::of_t
		#define X(subtype) < subtype
		MAGNITUDE_QUERY_FIRST
		#define X(subtype) , subtype
		MAGNITUDE_QUERY_REST
		>;
		#undef X

	DEFINE_VISITORS(magnitude_query, magnitude_query_subtype_list)

	//! A request to an agent for a magnitude.
	struct magnitude_query : element<magnitude_query_subtype_list>
	{
		virtual ~magnitude_query() = default;
	};

	DEFINE_ELEMENT_BASE(magnitude_query, magnitude_query)

	#define X(subtype) struct subtype : magnitude_query_base<subtype> {};
	MAGNITUDE_QUERY_FIRST
	MAGNITUDE_QUERY_REST
	#undef X
}
