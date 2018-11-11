//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "world/light_source.hpp"
#include "world/region.hpp"

namespace ql {
	namespace {
		constexpr auto range_per_lum = 1_span / 10.0_lum;
	}
	span light_source::range() const {
		return cancel::quantity_cast<span>(_luminance * range_per_lum);
	}

	lum light_source::luminance(region const& region, region_tile::point region_tile_coords) const {
		// Compute light source's luminance at this distance.
		span const distance = (region_tile_coords - _coords).length();
		double const occlusion = region.occlusion(_coords, region_tile_coords);
		return std::max(0.0_lum, _luminance - distance * range_per_lum * occlusion);
	}
}
