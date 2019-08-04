//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/perception.hpp"
#include "world/coordinates.hpp"
#include "world/section.hpp"

#include <array>
#include <optional>
#include <variant>
#include <vector>

namespace ql {
	struct being;

	//! Represents everything an agent can perceive about its being's environment.
	struct world_view {
		struct section_view {
			region_section::point coords;
			std::array<std::array<perception, section_diameter.value>, section_diameter.value> tile_perceptions;
		};

		struct entity_view {
			id id;
			perception perception;
		};

		std::vector<section_view> section_views;
		std::vector<entity_view> entity_views;
		location center;
		span visual_range;

		//! Constructs the world view of being with id @p being_id.
		world_view(id being_id);

		world_view(world_view const&) = default;
		world_view(world_view&&) = default;

		world_view& operator=(world_view const&) & = default;
		world_view& operator=(world_view&&) & = default;
	};
}
