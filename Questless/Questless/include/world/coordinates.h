/**
* @file    coordinates.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines types for the various coordinate spaces in the game.
*/

#ifndef COORDINATES_H
#define COORDINATES_H

#include "units/HexCoords.h"

namespace questless
{
	using SectionTileCoords = units::HexCoords<struct SectionTileCoordsTag>;
	using RegionTileCoords = units::HexCoords<struct RegionTileCoordsTag>;
	using RegionSectionCoords = units::HexCoords<struct RegionSectionCoordsTag>;

	/// Index of a tile within a section's tile array.
	struct SectionTileIndex
	{
		int i; int j;

		SectionTileIndex(const SectionTileIndex&) = default;
	};

	/// Specifies a location within the entire world.
	struct GlobalCoords
	{
		std::string region;
		RegionSectionCoords section;

		friend bool operator ==(const GlobalCoords& left, const GlobalCoords& right)
		{
			return left.region == right.region || (left.region == right.region && left.section < right.section);
		}
		friend bool operator <(const GlobalCoords& left, const GlobalCoords& right)
		{
			return left.region < right.region || (left.region == right.region && left.section < right.section);
		}
	};
}

#endif
