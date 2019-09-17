//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "bounded/nonnegative.hpp"
#include "quantities/distance.hpp"
#include "quantities/misc.hpp"
#include "utility/io.hpp"

namespace ql::stats {
	//! Represents a being's vision-related stats.
	struct vision {
		//! The maximum perception that can result from this vision.
		nonnegative<perception> acuity = 0_perception;

		//! The minimum illuminance at which this vision functions optimally.
		nonnegative<ql::lum> min_illuminance = 0_lum;

		//! The maximum illuminance at which this vision functions optimally.
		nonnegative<ql::lum> max_illuminance = 0_lum;

		//! The rate at which perception is decreased when below min. illuminance.
		nonnegative<perception_per_lum> darkness_penalty = 0_perception / 1_lum;

		//! The rate effective acuity is decreased when above max. illuminance.
		nonnegative<perception_per_lum> glare_penalty = 0_perception / 1_lum;
	};
}
