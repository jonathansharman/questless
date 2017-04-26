//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>

namespace questless
{
	struct DirectionQueryMeleeAttack;

	//! Visitor type for direction queries.
	struct DirectionQueryVisitor
	{
		virtual ~DirectionQueryVisitor() = default;

		virtual void visit(DirectionQueryMeleeAttack const&) = 0;
	};

	//! A request to an agent for a direction.
	struct DirectionQuery
	{
		virtual ~DirectionQuery() = default;
		virtual void accept(DirectionQueryVisitor& visitor) = 0;
	};

	struct DirectionQueryMeleeAttack : DirectionQuery
	{
		void accept(DirectionQueryVisitor& visitor) override { visitor.visit(*this); }
	};
}
