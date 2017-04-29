//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/Visitor.h"

namespace questless
{
	using CountQueryConstVisitor = Visitor<>;

	//! A request to an agent for a count.
	struct CountQuery : public ConstElement<CountQueryConstVisitor>
	{
		virtual ~CountQuery() = default;
	};

	DEFINE_CONST_ELEMENT_BASE(CountQuery, CountQuery)
}
