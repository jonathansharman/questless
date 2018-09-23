//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/stats/vision.hpp"

using namespace units::math;

namespace ql::stats {
	double vision::visibility(ql::illuminance illuminance, int distance) const {
		constexpr double darkness_factor = 0.05;
		constexpr double glare_factor = 0.015;

		double const illuminance_penalty = illuminance < min_illuminance.value()
			? square(min_illuminance.value() - illuminance) / (1.0_darkness_tolerance + darkness_tolerance.value()) * darkness_factor // Too dark
			: illuminance > max_illuminance.value()
				? square(illuminance - max_illuminance.value()) / (1.0_glare_tolerance + glare_tolerance.value()) * glare_factor // Too bright
				: 0.0 // Ideal illuminance range
			;
		double const distance_penalty = distance * distance * _distance_factor;
		return acuity.value() / (1.0 + illuminance_penalty + distance_penalty);
	}
}
