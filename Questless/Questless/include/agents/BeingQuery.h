//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>

namespace questless
{
	struct BeingQueryHealTarget;

	//! Visitor type for being queries.
	struct BeingQueryVisitor
	{
		virtual ~BeingQueryVisitor() = default;

		virtual void visit(BeingQueryHealTarget const&) = 0;
	};

	//! A request to an agent to specify a being.
	struct BeingQuery
	{
		virtual ~BeingQuery() = default;
		virtual void accept(BeingQueryVisitor& visitor) = 0;
	};

	struct BeingQueryHealTarget : BeingQuery
	{
		void accept(BeingQueryVisitor& visitor) override { visitor.visit(*this); }
	};
}
