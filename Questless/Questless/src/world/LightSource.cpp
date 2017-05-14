//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "world/LightSource.h"
#include "world/Region.h"

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

	double LightSource::luminance(Region const& region, RegionTile::Point region_tile_coords) const
	{
		// Compute light source's luminance at this distance.
		int const distance = (region_tile_coords - _coords).length();
		double const occlusion = region.occlusion(_coords, region_tile_coords);
		double const result = _luminance / (1.0 + distance * distance * distance_factor) * occlusion;

		// Clamp values below the cutoff to zero.
		return std::abs(result) >= cutoff ? result : 0.0;
	}
}
