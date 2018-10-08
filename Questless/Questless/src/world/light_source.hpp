//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <cmath>
#include <functional>
#include <memory>

#include "coordinates.hpp"
#include "utility/id.hpp"

namespace ql {
	class region;

	class light_source {
	public:
		id<light_source> const id;

		//! @param coords The coordinates of this light source in its region.
		//! @param luminance How brightly this light source shines.
		light_source(region_tile::point coords, double luminance, ql::id<light_source> id = ql::id<light_source>::make())
			: id{id}, _coords{coords}, _luminance{luminance}
		{}

		//! The coordinates of this light source in its region.
		region_tile::point coords() const { return _coords; }

		//! The distance this light source's light reaches.
		span range() const;

		//! How brightly this light source shines at @p region_tile_coords in @p region.
		lum luminance(region const& region, region_tile::point region_tile_coords) const;
	private:
		region_tile::point _coords;
		lum _luminance;
	};
}
