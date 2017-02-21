/**
* @file    BeingQuery.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Types of being queries, i.e. requests to an agent to specify a being.
*/

#pragma once

#include <memory>

namespace questless
{
	struct BeingQueryHealTarget;

	struct BeingQueryVisitor
	{
		virtual ~BeingQueryVisitor() = default;

		virtual void visit(BeingQueryHealTarget const&) = 0;
	};

	struct BeingQuery
	{
		using ptr = std::unique_ptr<BeingQuery>;
		virtual ~BeingQuery() = default;
		virtual void accept(BeingQueryVisitor& visitor) = 0;
	};
	struct BeingQueryHealTarget : BeingQuery
	{
		void accept(BeingQueryVisitor& visitor) override { visitor.visit(*this); }
	};
}
