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

namespace questless
{
	class Game;
	class Region;
	class Being;

	namespace WorldViewK
	{
		const double low_perception_threshold = 20.0;
		const double medium_perception_threshold = 40.0;
		const double high_perception_threshold = 60.0;
		const double full_perception_threshold = 80.0;
	}

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
		WorldView(const Game& game, const Being& being);

		const vector<SectionView>& section_views() const { return _section_views; }
		const vector<BeingView>& being_views() const { return _being_views; }
		const Region& region() const { return _region; }
	private:
		vector<SectionView> _section_views;
		vector<BeingView> _being_views;
		const Region& _region;
	};
}

#endif
