//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/stats/Vision.h"
#include "units/constexpr-math.h"
using namespace units::constexpr_math;

namespace questless
{
	namespace
	{
		constexpr double distance_factor = 0.05;
	}

	int Vision::max_range() const
	{
		// Use a cutoff to avoid including many tiles with very low visibility.
		constexpr double cutoff = 20.0;

		return static_cast<int>(sqrt((acuity / cutoff - 1.0) / distance_factor));
	}

	double Vision::visibility(double illuminance, int distance) const
	{
		constexpr double darkness_factor = 0.05;
		constexpr double glare_factor = 0.05;

		double const illuminance_penalty = illuminance < ideal_illuminance
			? square(ideal_illuminance - illuminance) / (1.0 + darkness_tolerance) * darkness_factor // Too dark
			: square(illuminance - ideal_illuminance) / (1.0 + glare_tolerance) * glare_factor // Too bright
			;
		double const distance_penalty = distance * distance * distance_factor;
		return acuity / (1.0 + illuminance_penalty + distance_penalty);
	}
}
