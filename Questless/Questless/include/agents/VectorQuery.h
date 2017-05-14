//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor-pattern.h"

namespace questless
{
	struct VectorQueryMeleeAttack;

	using VectorQuerySubtypeList = type_list::of_t<VectorQueryMeleeAttack>;

	DEFINE_VISITORS(VectorQuery, VectorQuerySubtypeList)

	//! A request to an agent for a vector in region tile space.
	struct VectorQuery : public Element<VectorQuerySubtypeList>
	{
		virtual ~VectorQuery() = default;
	};

	DEFINE_ELEMENT_BASE(VectorQuery, VectorQuery)

	struct VectorQueryMeleeAttack : VectorQueryBase<VectorQueryMeleeAttack> {};
}
