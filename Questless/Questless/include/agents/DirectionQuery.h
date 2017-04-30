//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>

namespace questless
{
	struct DirectionQueryMeleeAttack;

	using DirectionQuerySubtypeList = type_list::of_t<DirectionQueryMeleeAttack>;

	DEFINE_VISITORS(DirectionQuery, DirectionQuerySubtypeList)

	//! A request to an agent for a direction.
	struct DirectionQuery : public Element<DirectionQuerySubtypeList>
	{
		virtual ~DirectionQuery() = default;
	};

	DEFINE_ELEMENT_BASE(DirectionQuery, DirectionQuery)

	struct DirectionQueryMeleeAttack : DirectionQueryBase<DirectionQueryMeleeAttack> {};
}
