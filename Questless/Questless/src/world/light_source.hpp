//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "coordinates.hpp"

#include "reg.hpp"

namespace ql {
	struct light_source {
		reg_ptr reg;

		id id;

		//! How brightly this light source shines.
		lum luminance;

		//! The distance this light source's light reaches.
		auto range() const -> pace;

		//! How brightly this light source shines at @p target_location.
		auto luminance_at(location target_location) const -> lum;
	};
}
