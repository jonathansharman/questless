//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "world/LightSource.h"

namespace questless
{
	namespace
	{
		constexpr double distance_factor = 0.25;

		// Use a cutoff to limit range.
		constexpr double cutoff = 1.0;
	}
	int LightSource::range() const
	{
		return static_cast<int>(sqrt((std::max(0.0, std::abs(_luminance) - cutoff)) / distance_factor)) + 1;
	}

	double LightSource::luminance(RegionTileCoords region_tile_coords) const
	{
		int const distance = _coords.distance_to(region_tile_coords);
		double const result = _luminance / (1.0 + distance * distance * distance_factor);
		return std::abs(result) >= cutoff ? result : 0.0;
	}
}
