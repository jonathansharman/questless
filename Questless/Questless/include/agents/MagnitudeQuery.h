//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor-pattern.h"

namespace questless
{
	struct MagnitudeQueryHeal;
	struct MagnitudeQueryLightningBolt;
	struct MagnitudeQueryWaitTime;

	using MagnitudeQuerySubtypeList = type_list::of_t
		< MagnitudeQueryHeal
		, MagnitudeQueryLightningBolt
		, MagnitudeQueryWaitTime
		>;

	DEFINE_VISITORS(MagnitudeQuery, MagnitudeQuerySubtypeList)

	//! A request to an agent for a magnitude.
	struct MagnitudeQuery : Element<MagnitudeQuerySubtypeList>
	{
		virtual ~MagnitudeQuery() = default;
	};

	DEFINE_ELEMENT_BASE(MagnitudeQuery, MagnitudeQuery)

	struct MagnitudeQueryHeal : MagnitudeQueryBase<MagnitudeQueryHeal> {};
	struct MagnitudeQueryLightningBolt : MagnitudeQueryBase<MagnitudeQueryLightningBolt> {};
	struct MagnitudeQueryWaitTime : MagnitudeQueryBase<MagnitudeQueryWaitTime> {};
}
