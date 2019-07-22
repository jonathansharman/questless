//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "bounded/static.hpp"
#include "entities/beings/stats/vision.hpp"
#include "quantities/misc.hpp"
#include "reg.hpp"
#include "world/coordinates.hpp"

#include "cancel/quantity.hpp"

#include <vector>

namespace ql {
	constexpr auto min_perception = 0_perception;
	constexpr auto max_perception = 100_perception;

	using bounded_perception = static_bounded<perception, min_perception, max_perception>;

	//! The maximum possible distance a being with vision list @p vision_sources could see.
	span max_visual_range(std::vector<stats::vision> const& vision_sources);

	//! The perception of the @p target tile by the being with ID @p perceptor_id.
	perception perception_of(ent perceptor_id, region_tile::point target);
}
