//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <cmath>
#include <functional>
#include <memory>

#include "coordinates.h"
#include "utility/Id.h"

namespace questless
{
	class Region;

	class LightSource
	{
	public:
		Id<LightSource> const id;

		//! @param coords The coordinates of this light source in its region.
		//! @param luminance How brightly this light source shines.
		LightSource(RegionTile::Point coords, double luminance, Id<LightSource> id = Id<LightSource>::make())
			: id{id}, _coords{coords}, _luminance{luminance}
		{}

		//! The coordinates of this light source in its region.
		RegionTile::Point coords() const { return _coords; }

		//! The distance this light source's light reaches.
		int range() const;

		//! How brightly this light source shines at @p region_tile_coords in @p region.
		double luminance(Region const& region, RegionTile::Point region_tile_coords) const;
	private:
		RegionTile::Point _coords;
		double _luminance;
	};
}
