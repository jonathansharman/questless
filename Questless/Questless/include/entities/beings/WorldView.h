//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <vector>
#include <array>
#include <optional>

#include "world/coordinates.h"
#include "world/Section.h"
#include "units/GameRect.h"
#include "utility/Id.h"

namespace questless
{
	class Region;
	class Being;
	class Object;

	//! Represents everything an agent can perceive about its environment.
	class WorldView
	{
	public:
		struct SectionView
		{
			RegionSectionCoords coords;
			std::array<std::array<double, Section::diameter>, Section::diameter> tile_visibilities; //!< Indices are r-major.
		};

		//! Represents discrete levels of perception of an entity.
		enum class PerceptionLevel
			{ low    // Can tell there's an entity there but can't identify it.
			, medium // Can tell what type of entity is there.
			, high   // Can tell what type of entity is there, as well as many attributes (e.g., health, mana, eqipped items).
			, full   // Knows everything about the being.
			};

		struct BeingView
		{
			Id<Being> id;
			PerceptionLevel perception;

			BeingView(Id<Being> id, PerceptionLevel perception) : id{id}, perception{perception} {}
		};

		struct ObjectView
		{
			Id<Object> id;
			PerceptionLevel perception;

			ObjectView(Id<Object> id, PerceptionLevel perception) : id{id}, perception{perception} {}
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
		std::vector<BeingView> const& being_views() const { return _being_views; }
		std::vector<ObjectView> const& object_views() const { return _object_views; }
		Region const& region() const { return _region; }

		//! A bounding rectangle around the visible tiles or nullopt if initialized with find_bounds set to false or if no tiles are visible.
		std::optional<units::GameRect> bounds() const { return _bounds; }
	private:
		static constexpr double _low_perception_threshold = 20.0;
		static constexpr double _medium_perception_threshold = 40.0;
		static constexpr double _high_perception_threshold = 60.0;
		static constexpr double _full_perception_threshold = 80.0;

		std::vector<SectionView> _section_views;
		std::vector<BeingView> _being_views;
		std::vector<ObjectView> _object_views;
		std::reference_wrapper<const Region> _region;

		std::optional<units::GameRect> _bounds;
	};
}
