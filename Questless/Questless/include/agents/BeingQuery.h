//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/Visitor.h"

namespace questless
{
	struct BeingQueryHealTarget;

	using BeingQueryMutableVisitor = Visitor<BeingQueryHealTarget>;

	//! A request to an agent to specify a being.
	struct BeingQuery : public MutableElement<BeingQueryMutableVisitor>
	{
		virtual ~BeingQuery() = default;
	};

	DEFINE_MUTABLE_ELEMENT_BASE(BeingQuery, BeingQuery)

	struct BeingQueryHealTarget : BeingQueryMutableBase<BeingQueryHealTarget> {};
}
