//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/stats/vision.hpp"
using namespace units::math;

namespace ql
{
	double vision::visibility(double illuminance, int distance) const
	{
		constexpr double darkness_factor = 0.05;
		constexpr double glare_factor = 0.015;

		double const illuminance_penalty = illuminance < ideal_illuminance
			? square(ideal_illuminance - illuminance) / (1.0 + darkness_tolerance) * darkness_factor // Too dark
			: square(illuminance - ideal_illuminance) / (1.0 + glare_tolerance) * glare_factor // Too bright
			;
		double const distance_penalty = distance * distance * _distance_factor;
		return acuity / (1.0 + illuminance_penalty + distance_penalty);
	}
}
