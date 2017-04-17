//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>

namespace questless
{
	struct MagnitudeQueryHeal;
	struct MagnitudeQueryLightningBolt;
	struct MagnitudeQueryWaitTime;

	//! Visitor type for magnitude queries.
	struct MagnitudeQueryVisitor
	{
		virtual ~MagnitudeQueryVisitor() = default;

		virtual void visit(MagnitudeQueryHeal const&) = 0;
		virtual void visit(MagnitudeQueryLightningBolt const&) = 0;
		virtual void visit(MagnitudeQueryWaitTime const&) = 0;
	};

	//! A request to an agent for a magnitude.
	struct MagnitudeQuery
	{
		using uptr = std::unique_ptr<MagnitudeQuery>;
		virtual ~MagnitudeQuery() = default;
		virtual void accept(MagnitudeQueryVisitor& visitor) = 0;
	};
	struct MagnitudeQueryHeal : MagnitudeQuery
	{
		void accept(MagnitudeQueryVisitor& visitor) override { visitor.visit(*this); }
	};
	struct MagnitudeQueryLightningBolt : MagnitudeQuery
	{
		void accept(MagnitudeQueryVisitor& visitor) override { visitor.visit(*this); }
	};
	struct MagnitudeQueryWaitTime : MagnitudeQuery
	{
		void accept(MagnitudeQueryVisitor& visitor) override { visitor.visit(*this); }
	};
}
