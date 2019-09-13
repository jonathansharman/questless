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
		struct tile_view {
			id id;
			perception perception;

			// Position is included here (redundantly) for efficiency because it's required in various places.
			view::point position;
		};

		struct entity_view {
			id id;
			perception perception;

			// Position is included here (redundantly) for efficiency because it's required in various places.
			view::point position;
		};

		std::vector<tile_view> tile_views;
		std::vector<entity_view> entity_views;
		location center;
		span visual_range;

		//! Constructs the world view of the being with id @p viewer_id.
		world_view(id viewer_id);

		world_view(world_view const&) = default;
		world_view(world_view&&) = default;

		auto operator=(world_view const&) -> world_view& = default;
		auto operator=(world_view &&) -> world_view& = default;
	};
}
