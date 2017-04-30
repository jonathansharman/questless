//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor_pattern.h"

namespace questless
{
	struct TileQueryLightningBoltTarget;
	struct TileQueryRangedAttackTarget;
	struct TileQueryTeleportTarget;

	using TileQuerySubtypeList = type_list::of_t
		< TileQueryLightningBoltTarget
		, TileQueryRangedAttackTarget
		, TileQueryTeleportTarget
		>;

	DEFINE_VISITORS(TileQuery, TileQuerySubtypeList)

	//! A request to an agent for tile coordinates.
	struct TileQuery : public Element<TileQuerySubtypeList>
	{
		virtual ~TileQuery() = default;
	};

	DEFINE_ELEMENT_BASE(TileQuery, TileQuery)

	struct TileQueryLightningBoltTarget : TileQueryBase<TileQueryLightningBoltTarget> {};
	struct TileQueryRangedAttackTarget : TileQueryBase<TileQueryRangedAttackTarget>
	{
		int range;
		TileQueryRangedAttackTarget(int range) : range{ range } {}
	};
	struct TileQueryTeleportTarget : TileQueryBase<TileQueryTeleportTarget> {};
}
