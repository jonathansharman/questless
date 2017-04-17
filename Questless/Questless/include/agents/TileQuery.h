//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>

namespace questless
{
	struct TileQueryLightningBoltTarget;
	struct TileQueryRangedAttackTarget;
	struct TileQueryTeleportTarget;

	//! Visitor type for tile queries.
	struct TileQueryVisitor
	{
		virtual ~TileQueryVisitor() = default;

		virtual void visit(TileQueryLightningBoltTarget const&) = 0;
		virtual void visit(TileQueryRangedAttackTarget const&) = 0;
		virtual void visit(TileQueryTeleportTarget const&) = 0;
	};

	//! A request to an agent for tile coordinates.
	struct TileQuery
	{
		using uptr = std::unique_ptr<TileQuery>;
		virtual ~TileQuery() = default;
		virtual void accept(TileQueryVisitor& visitor) = 0;
	};

	struct TileQueryLightningBoltTarget : TileQuery
	{
		void accept(TileQueryVisitor& visitor) override { visitor.visit(*this); }
	};
	struct TileQueryRangedAttackTarget : TileQuery
	{
		int range;
		TileQueryRangedAttackTarget(int range) : range{ range } {}
		void accept(TileQueryVisitor& visitor) override { visitor.visit(*this); }
	};
	struct TileQueryTeleportTarget : TileQuery
	{
		void accept(TileQueryVisitor& visitor) override { visitor.visit(*this); }
	};
}
