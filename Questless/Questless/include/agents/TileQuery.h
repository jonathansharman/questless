//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/Visitor.h"

namespace questless
{
	struct TileQueryLightningBoltTarget;
	struct TileQueryRangedAttackTarget;
	struct TileQueryTeleportTarget;

	using TileQueryMutableVisitor = Visitor
		< TileQueryLightningBoltTarget
		, TileQueryRangedAttackTarget
		, TileQueryTeleportTarget
		>;

	using TileQueryConstVisitor = Visitor
		< TileQueryLightningBoltTarget const
		, TileQueryRangedAttackTarget const
		, TileQueryTeleportTarget const
		>;

	//! A request to an agent for tile coordinates.
	struct TileQuery : public Element<TileQueryMutableVisitor, TileQueryConstVisitor>
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
