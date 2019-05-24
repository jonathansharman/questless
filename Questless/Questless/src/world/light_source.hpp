//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "coordinates.hpp"
#include "reg.hpp"

namespace ql {
	struct light_source {
		ent id;

		//! How brightly this light source shines.
		lum luminance;

		//! The distance this light source's light reaches.
		span range() const;

		//! How brightly this light source shines at @p target_location.
		lum luminance_at(location target_location) const;
	};
}
