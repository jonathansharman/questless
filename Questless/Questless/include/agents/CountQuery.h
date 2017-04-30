//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor_pattern.h"

namespace questless
{
	using CountQuerySubtypeList = type_list::Empty;

	DEFINE_VISITORS(CountQuery, CountQuerySubtypeList)

	//! A request to an agent for a count.
	struct CountQuery : public Element<CountQuerySubtypeList>
	{
		virtual ~CountQuery() = default;
	};

	DEFINE_ELEMENT_BASE(CountQuery, CountQuery)
}
