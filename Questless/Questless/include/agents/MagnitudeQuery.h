/**
* @file    MagnitudeQuery.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Requests to an agent for a magnitude.
*/

#pragma once

#include <memory>

namespace questless
{
	struct MagnitudeQueryHeal;
	struct MagnitudeQueryLightningBolt;
	struct MagnitudeQueryWaitTime;

	struct MagnitudeQueryVisitor
	{
		virtual ~MagnitudeQueryVisitor() = default;

		virtual void visit(MagnitudeQueryHeal const&) = 0;
		virtual void visit(MagnitudeQueryLightningBolt const&) = 0;
		virtual void visit(MagnitudeQueryWaitTime const&) = 0;
	};

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
