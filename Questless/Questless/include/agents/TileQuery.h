//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor-pattern.h"

namespace questless
{
	struct TileQueryRangedAttackTarget;
	struct TileQueryShockTarget;
	struct TileQueryTeleportTarget;

	using TileQuerySubtypeList = type_list::of_t
		< TileQueryRangedAttackTarget
		, TileQueryShockTarget
		, TileQueryTeleportTarget
		>;

	DEFINE_VISITORS(TileQuery, TileQuerySubtypeList)

	//! A request to an agent for tile coordinates.
	struct TileQuery : public Element<TileQuerySubtypeList>
	{
		virtual ~TileQuery() = default;
	};

	DEFINE_ELEMENT_BASE(TileQuery, TileQuery)

	struct TileQueryRangedAttackTarget : TileQueryBase<TileQueryRangedAttackTarget>
	{
		int range;
		TileQueryRangedAttackTarget(int range) : range{ range } {}
	};
	struct TileQueryShockTarget : TileQueryBase<TileQueryShockTarget> {};
	struct TileQueryTeleportTarget : TileQueryBase<TileQueryTeleportTarget> {};
}
