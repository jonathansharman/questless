//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/Visitor.h"

namespace questless
{
	using CountQueryMutableVisitor = Visitor<>;
	using CountQueryConstVisitor = Visitor<>;

	//! A request to an agent for a count.
	struct CountQuery : public Element<CountQueryMutableVisitor, CountQueryConstVisitor>
	{
		virtual ~CountQuery() = default;
	};

	DEFINE_ELEMENT_BASE(CountQuery, CountQuery)
}
