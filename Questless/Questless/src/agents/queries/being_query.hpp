//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor_pattern.hpp"

namespace ql {
	struct being_query_heal_target;

	using being_query_subtype_list = type_list::of_t<being_query_heal_target>;

	DEFINE_VISITORS(being_query, being_query_subtype_list)

	//! A request to an agent to specify a being.
	struct being_query : public element<being_query_subtype_list> {
		virtual ~being_query() = default;
	};

	DEFINE_ELEMENT_BASE(being_query, being_query)

	struct being_query_heal_target : being_query_base<being_query_heal_target> {};
}
