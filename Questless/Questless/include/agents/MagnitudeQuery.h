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
	struct MagnitudeQueryWaitTime;
	struct MagnitudeQueryLightningBolt;
	struct MagnitudeQueryHeal;

	struct MagnitudeQueryVisitor
	{
		virtual ~MagnitudeQueryVisitor() = default;

		virtual void visit(MagnitudeQueryWaitTime const&) = 0;
		virtual void visit(MagnitudeQueryLightningBolt const&) = 0;
		virtual void visit(MagnitudeQueryHeal const&) = 0;
	};

	struct MagnitudeQuery
	{
		using ptr = std::unique_ptr<MagnitudeQuery>;
		virtual ~MagnitudeQuery() = default;
		virtual void accept(MagnitudeQueryVisitor& visitor) = 0;
	};
	struct MagnitudeQueryWaitTime : MagnitudeQuery
	{
		void accept(MagnitudeQueryVisitor& visitor) override { visitor.visit(*this); }
	};
	struct MagnitudeQueryLightningBolt : MagnitudeQuery
	{
		void accept(MagnitudeQueryVisitor& visitor) override { visitor.visit(*this); }
	};
	struct MagnitudeQueryHeal : MagnitudeQuery
	{
		void accept(MagnitudeQueryVisitor& visitor) override { visitor.visit(*this); }
	};
}
