//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "world/light_source.hpp"
#include "world/region.hpp"

namespace ql {
	namespace {
		constexpr double distance_factor = 0.25;

		// Use a cutoff to limit range.
		constexpr double cutoff = 1.0;
	}
	int light_source::range() const {
		return static_cast<int>(sqrt((std::max(0.0, std::abs(_luminance) - cutoff)) / distance_factor)) + 1;
	}

	double light_source::luminance(region const& region, region_tile::point region_tile_coords) const {
		// Compute light source's luminance at this distance.
		int const distance = (region_tile_coords - _coords).length();
		double const occlusion = region.occlusion(_coords, region_tile_coords);
		double const result = _luminance / (1.0 + distance * distance * distance_factor) * occlusion;

		// Clamp values below the cutoff to zero.
		return std::abs(result) >= cutoff ? result : 0.0;
	}
}
