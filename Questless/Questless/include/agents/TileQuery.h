/**
* @file    TileQuery.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Requests to an agent for tile coordinates.
*/

#pragma once

#include <memory>

namespace questless
{
	struct TileQueryRangedAttackTarget;
	struct TileQueryLightningBoltTarget;
	struct TileQueryTeleportTarget;

	struct TileQueryVisitor
	{
		virtual ~TileQueryVisitor() = default;

		virtual void visit(TileQueryRangedAttackTarget const&) = 0;
		virtual void visit(TileQueryLightningBoltTarget const&) = 0;
		virtual void visit(TileQueryTeleportTarget const&) = 0;
	};

	struct TileQuery
	{
		using ptr = std::unique_ptr<TileQuery>;
		virtual ~TileQuery() = default;
		virtual void accept(TileQueryVisitor& visitor) = 0;
	};
	struct TileQueryRangedAttackTarget : TileQuery
	{
		int range;
		TileQueryRangedAttackTarget(int range) : range{range} {}
		void accept(TileQueryVisitor& visitor) override { visitor.visit(*this); }
	};
	struct TileQueryLightningBoltTarget : TileQuery
	{
		void accept(TileQueryVisitor& visitor) override { visitor.visit(*this); }
	};
	struct TileQueryTeleportTarget : TileQuery
	{
		void accept(TileQueryVisitor& visitor) override { visitor.visit(*this); }
	};
}
