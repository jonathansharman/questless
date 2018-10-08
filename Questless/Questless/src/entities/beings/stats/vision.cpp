//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/stats/vision.hpp"

using namespace units::math;

namespace ql::stats {
	perception::level vision::perception(ql::lum illuminance, span distance, double occlusion) const {
		ql::acuity const illuminance_penalty = illuminance < min_illuminance.value()
			? (min_illuminance.value() - illuminance) * darkness_penalty.value() // Too dark
			: illuminance > max_illuminance.value()
				? (illuminance - max_illuminance.value()) * glare_penalty.value() // Too bright
				: 0.0_acuity // Ideal illuminance range
			;
		ql::acuity const distance_penalty = distance * acuity_loss_per_span;
		ql::acuity const effective_acuity = acuity.value() - illuminance_penalty - distance_penalty;
		constexpr auto perception_per_acuity = 1.0_perception / 1.0_acuity;
		return effective_acuity * perception_per_acuity * occlusion;
	}
}
