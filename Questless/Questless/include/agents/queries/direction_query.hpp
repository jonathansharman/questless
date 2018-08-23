//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor_pattern.hpp"

namespace ql {
	using drection_query_subtype_list = type_list::of_t<>;

	DEFINE_VISITORS(direction_query, drection_query_subtype_list)

	//! A request to an agent for a direction.
	struct direction_query : public element<drection_query_subtype_list> {
		virtual ~direction_query() = default;
	};

	DEFINE_ELEMENT_BASE(direction_query, direction_query)
}
