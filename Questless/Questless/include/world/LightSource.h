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
	class LightSource
	{
	public:
		using uptr = std::unique_ptr<LightSource>;
		using ref = std::reference_wrapper<LightSource>;
		using cref = std::reference_wrapper<LightSource const>;

		Id<LightSource> const id;

		//! @param coords The coordinates of this light source in its region.
		//! @param luminance How brightly this light source shines.
		LightSource(RegionTileCoords coords, double luminance, Id<LightSource> id = Id<LightSource>::make())
			: id{id}, _coords{coords}, _luminance{luminance}
		{}

		//! The coordinates of this light source in its region.
		RegionTileCoords coords() const { return _coords; }

		//! The distance this light source's light reaches.
		int range() const;

		//! How brightly this light source shines at @p region_tile_coords.
		double luminance(RegionTileCoords region_tile_coords) const;
	private:
		RegionTileCoords _coords;
		double _luminance;
	};
}
