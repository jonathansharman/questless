//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/Visitor.h"

namespace questless
{
	struct BeingQueryHealTarget;

	using BeingQueryMutableVisitor = Visitor<BeingQueryHealTarget>;
	using BeingQueryConstVisitor = Visitor<BeingQueryHealTarget const>;

	//! A request to an agent to specify a being.
	struct BeingQuery : public Element<BeingQueryMutableVisitor, BeingQueryConstVisitor>
	{
		virtual ~BeingQuery() = default;
	};

	DEFINE_ELEMENT_BASE(BeingQuery, BeingQuery)

	struct BeingQueryHealTarget : BeingQueryBase<BeingQueryHealTarget> {};
}
