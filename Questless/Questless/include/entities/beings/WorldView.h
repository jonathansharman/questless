//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <array>
#include <optional>
#include <variant>
#include <vector>

#include "entities/Perception.h"
#include "units/GameSpace.h"
#include "utility/Id.h"
#include "world/coordinates.h"
#include "world/Section.h"

namespace questless
{
	class Region;
	class Being;
	class Object;

	//! Represents everything an agent can perceive about its being's environment.
	class WorldView
	{
	public:
		struct SectionView
		{
			RegionSection::Point coords;
			std::array<std::array<Perception, Section::diameter>, Section::diameter> tile_perceptions;
		};

		struct EntityView
		{
			std::variant<Id<Being>, Id<Object>> id;
			Perception perception;

			EntityView(std::variant<Id<Being>, Id<Object>> id, Perception perception)
				: id{id}, perception{perception}
			{}
		};

		//! Constructs the world view of the given being.
		//! @param being The being whose perspective this world view represents.
		//! @param find_bounds If true, the world view will find a bounding rectangle in world space around the visible tiles.
		WorldView(Being const& being, bool find_bounds);
		
		//! Copy constructor.
		WorldView(WorldView const&) = default;
		
		//! Move constructor.
		WorldView(WorldView&&) = default;

		//! Copy assignment operator.
		WorldView& operator =(WorldView const&) & = default;

		//! Move assignment operator.
		WorldView& operator =(WorldView&&) & = default;

		std::vector<SectionView> const& section_views() const { return _section_views; }
		std::vector<EntityView> const& entity_views() const { return _entity_views; }
		Region const& region() const { return _region; }

		//! A bounding box around the visible tiles or nullopt if initialized with find_bounds set to false or if no tiles are visible.
		std::optional<units::GameSpace::Box> bounds() const { return _bounds; }
	private:
		std::vector<SectionView> _section_views;
		std::vector<EntityView> _entity_views;
		std::reference_wrapper<const Region> _region;

		std::optional<units::GameSpace::Box> _bounds;
	};
}
