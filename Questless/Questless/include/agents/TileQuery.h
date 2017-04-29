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

	using TileQueryConstVisitor = Visitor
		< TileQueryLightningBoltTarget const
		, TileQueryRangedAttackTarget const
		, TileQueryTeleportTarget const
		>;

	//! A request to an agent for tile coordinates.
	struct TileQuery : public ConstElement<TileQueryConstVisitor>
	{
		virtual ~TileQuery() = default;
	};

	DEFINE_CONST_ELEMENT_BASE(TileQuery, TileQuery)

	struct TileQueryLightningBoltTarget : TileQueryConstBase<TileQueryLightningBoltTarget> {};
	struct TileQueryRangedAttackTarget : TileQueryConstBase<TileQueryRangedAttackTarget>
	{
		int range;
		TileQueryRangedAttackTarget(int range) : range{ range } {}
	};
	struct TileQueryTeleportTarget : TileQueryConstBase<TileQueryTeleportTarget> {};
}
