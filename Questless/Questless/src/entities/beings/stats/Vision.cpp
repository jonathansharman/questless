//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/stats/Vision.h"

namespace questless
{
	namespace
	{
		constexpr double distance_factor = 0.05;
	}

	int Vision::max_range() const
	{
		// Use a cutoff to avoid including many tiles with very low visibility.
		double constexpr cutoff = 20.0;

		return static_cast<int>(sqrt((acuity / cutoff - 1.0) / distance_factor));
	}

	double Vision::visibility(double illuminance, int distance) const
	{
		double constexpr illuminance_factor = 0.25;

		double const luminance_penalty = (illuminance - ideal_illuminance) * (illuminance - ideal_illuminance) / illuminance_tolerance * illuminance_factor;
		double const distance_penalty = distance * distance * distance_factor;
		return acuity / (1.0 + luminance_penalty + distance_penalty);
	}
}
