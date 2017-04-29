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

	using MagnitudeQueryMutableVisitor = Visitor
		< MagnitudeQueryHeal
		, MagnitudeQueryLightningBolt
		, MagnitudeQueryWaitTime
		>;

	using MagnitudeQueryConstVisitor = Visitor
		< MagnitudeQueryHeal const
		, MagnitudeQueryLightningBolt const
		, MagnitudeQueryWaitTime const
		>;

	//! A request to an agent for a magnitude.
	struct MagnitudeQuery : Element<MagnitudeQueryMutableVisitor, MagnitudeQueryConstVisitor>
	{
		virtual ~MagnitudeQuery() = default;
	};

	DEFINE_ELEMENT_BASE(MagnitudeQuery, MagnitudeQuery)

	struct MagnitudeQueryHeal : MagnitudeQueryBase<MagnitudeQueryHeal> {};
	struct MagnitudeQueryLightningBolt : MagnitudeQueryBase<MagnitudeQueryLightningBolt> {};
	struct MagnitudeQueryWaitTime : MagnitudeQueryBase<MagnitudeQueryWaitTime> {};
}
