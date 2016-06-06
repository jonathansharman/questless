/**
* @file    WorldView.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the WorldView class, which represents all that an agent can perceive about its surrounding environment.
*/

#ifndef WORLD_VIEW
#define WORLD_VIEW

#include <vector>
using std::vector;
#include <array>
using std::array;
#include <memory>
using std::shared_ptr;

#include "utility/constants.h"
#include "utility/hex-utilities.h"
#include "utility/optional.h"

namespace questless
{
	class Game;
	class Region;
	class Being;

	class WorldView
	{
	public:
		struct SectionView
		{
			HexCoords coords;
			array<array<double, section_diameter>, section_diameter> tile_visibilities;
		};

		struct BeingView
		{
			enum class Perception
			{ none   // Imperceptible.
			, low    // Can tell there's something there but can't identify it.
			, medium // Can tell what type of being is there.
			, high   // Can tell what type of being is there, as well as its health, mana, and equipped items.
			, full   // Knows everything about the being.
			};

			Being* being;
			Perception perception;
		};
		
		/// Copy constructor.
		WorldView(const WorldView&) = default;
		
		/// Move constructor.
		WorldView(WorldView&&) = default;

		/// Copy assignment operator.
		WorldView& operator =(const WorldView&) = default;

		/// Move assignment operator.
		WorldView& operator =(WorldView&&) = default;

		/// Constructs the world view of the given being.
		/// @param game The game object, used for determining the being's surroundings.
		/// @param being The being whose perspective this world view represents.
		/// @param find_bounds If true, the world view will find a bounding rectangle in world space around the visible tiles.
		WorldView(const Game& game, const Being& being, bool find_bounds);

		const vector<SectionView>& section_views() const { return _section_views; }
		const vector<BeingView>& being_views() const { return _being_views; }
		const Region& region() const { return _region; }

		/// @return A bounding rectangle around the visible tiles or nullopt if initialized with find_bounds set to false or if no tiles are visible.
		optional<sdl::Rect> bounds() const { return _bounds; }
	private:
		static constexpr double _low_perception_threshold = 20.0;
		static constexpr double _medium_perception_threshold = 40.0;
		static constexpr double _high_perception_threshold = 60.0;
		static constexpr double _full_perception_threshold = 80.0;

		vector<SectionView> _section_views;
		vector<BeingView> _being_views;
		const Region& _region;

		optional<sdl::Rect> _bounds;
	};
}

#endif
