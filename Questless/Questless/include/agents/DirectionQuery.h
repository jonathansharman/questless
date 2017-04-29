//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>

namespace questless
{
	struct DirectionQueryMeleeAttack;

	using DirectionQueryMutableVisitor = Visitor<DirectionQueryMeleeAttack>;

	using DirectionQueryConstVisitor = Visitor<DirectionQueryMeleeAttack const>;

	//! A request to an agent for a direction.
	struct DirectionQuery : public Element<DirectionQueryMutableVisitor, DirectionQueryConstVisitor>
	{
		virtual ~DirectionQuery() = default;
	};

	DEFINE_ELEMENT_BASE(DirectionQuery, DirectionQuery)

	struct DirectionQueryMeleeAttack : DirectionQueryBase<DirectionQueryMeleeAttack> {};
}
