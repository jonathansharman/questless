//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "bounded/static.hpp"
#include "entities/beings/stats/vision.hpp"
#include "quantities/misc.hpp"
#include "reg.hpp"
#include "world/coordinates.hpp"

#include "cancel/quantity.hpp"

#include <vector>

namespace ql {
	static constexpr auto max_perception = 100_perception;

	//! The maximum possible distance a being with vision list @p vision_sources could see.
	auto max_visual_range(std::vector<stats::vision> const& vision_sources) -> pace;

	//! The nonnegative perception of the @p target tile by the being with ID @p perceptor_id.
	auto perception_of(reg& reg, id perceptor_id, tile_hex::point target) -> perception;
}
