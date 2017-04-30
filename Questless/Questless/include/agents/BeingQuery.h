//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor-pattern.h"

namespace questless
{
	struct BeingQueryHealTarget;

	using BeingQuerySubtypeList = type_list::of_t<BeingQueryHealTarget>;

	DEFINE_VISITORS(BeingQuery, BeingQuerySubtypeList)

	//! A request to an agent to specify a being.
	struct BeingQuery : public Element<BeingQuerySubtypeList>
	{
		virtual ~BeingQuery() = default;
	};

	DEFINE_ELEMENT_BASE(BeingQuery, BeingQuery)

	struct BeingQueryHealTarget : BeingQueryBase<BeingQueryHealTarget> {};
}
