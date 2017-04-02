/**
* @file    DirectionQuery.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Requests to an agent for a direction.
*/

#pragma once

#include <memory>

namespace questless
{
	struct DirectionQueryMeleeAttack;

	struct DirectionQueryVisitor
	{
		virtual ~DirectionQueryVisitor() = default;

		virtual void visit(DirectionQueryMeleeAttack const&) = 0;
	};

	struct DirectionQuery
	{
		using uptr = std::unique_ptr<DirectionQuery>;
		virtual ~DirectionQuery() = default;
		virtual void accept(DirectionQueryVisitor& visitor) = 0;
	};
	struct DirectionQueryMeleeAttack : DirectionQuery
	{
		void accept(DirectionQueryVisitor& visitor) override { visitor.visit(*this); }
	};
}
