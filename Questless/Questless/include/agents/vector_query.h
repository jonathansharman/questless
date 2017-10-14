//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor_pattern.h"

namespace ql
{
	struct vector_query_melee_attack;

	using vector_query_subtype_list = type_list::of_t<vector_query_melee_attack>;

	DEFINE_VISITORS(vector_query, vector_query_subtype_list)

	//! A request to an agent for a vector in region tile space.
	struct vector_query : public element<vector_query_subtype_list>
	{
		virtual ~vector_query() = default;
	};

	DEFINE_ELEMENT_BASE(vector_query, vector_query)

	struct vector_query_melee_attack : vector_query_base<vector_query_melee_attack> {};
}
