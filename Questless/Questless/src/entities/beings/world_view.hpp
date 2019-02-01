//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <array>
#include <optional>
#include <variant>
#include <vector>

#include "entities/perception.hpp"
#include "utility/id.hpp"
#include "world/coordinates.hpp"
#include "world/section.hpp"
#include "world/world.hpp"

namespace ql {
	class region;
	class being;
	class object;

	//! Represents everything an agent can perceive about its being's environment.
	class world_view {
	public:
		struct section_view {
			region_section::point coords;
			std::array<std::array<perception::bounded_level, section::diameter.value>, section::diameter.value> tile_perceptions;
		};

		struct entity_view {
			std::variant<id<being>, id<object>> id;
			perception::bounded_level perception;
		};

		//! Constructs the world view of the given being.
		//! @param being The being whose perspective this world view represents.
		world_view(being const& being);
		
		//! Copy constructor.
		world_view(world_view const&) = default;
		
		//! Move constructor.
		world_view(world_view&&) = default;

		//! Copy assignment operator.
		world_view& operator =(world_view const&) & = default;

		//! Move assignment operator.
		world_view& operator =(world_view&&) & = default;

		std::vector<section_view> const& section_views() const { return _section_views; }
		std::vector<entity_view> const& entity_views() const { return _entity_views; }
		ql::region const& region() const { return _region; }
		region_tile::point origin() const { return _origin; }
		span visual_range() const { return _visual_range; }
	private:
		std::vector<section_view> _section_views;
		std::vector<entity_view> _entity_views;
		std::reference_wrapper<const ql::region> _region;
		region_tile::point _origin;
		span _visual_range;
	};
}
