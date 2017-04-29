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

	using MagnitudeQueryConstVisitor = Visitor
		< MagnitudeQueryHeal const
		, MagnitudeQueryLightningBolt const
		, MagnitudeQueryWaitTime const
		>;

	//! A request to an agent for a magnitude.
	struct MagnitudeQuery : ConstElement<MagnitudeQueryConstVisitor>
	{
		virtual ~MagnitudeQuery() = default;
	};

	DEFINE_CONST_ELEMENT_BASE(MagnitudeQuery, MagnitudeQuery)

	struct MagnitudeQueryHeal : MagnitudeQueryConstBase<MagnitudeQueryHeal> {};
	struct MagnitudeQueryLightningBolt : MagnitudeQueryConstBase<MagnitudeQueryLightningBolt> {};
	struct MagnitudeQueryWaitTime : MagnitudeQueryConstBase<MagnitudeQueryWaitTime> {};
}
