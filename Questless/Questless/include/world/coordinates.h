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

#include "utility/hex-utilities.h"

namespace questless
{
	/// Coordinates within the space of tiles in a section.
	struct SectionTileCoords
	{
		HexCoords hex;

		friend bool operator <(const SectionTileCoords& left, const SectionTileCoords& right)
		{
			return left.hex < right.hex;
		}
	};
	/// Index of a tile within a section's tile array.
	struct SectionTileIndex
	{
		int i; int j;
	};

	/// Coordinates within the space of tiles in a region.
	struct RegionTileCoords
	{
		HexCoords hex;

		friend bool operator <(const RegionTileCoords& left, const RegionTileCoords& right)
		{
			return left.hex < right.hex;
		}
	};
	/// Coordinates within the space of sections in a region.
	struct RegionSectionCoords
	{
		HexCoords hex;

		friend bool operator <(const RegionSectionCoords& left, const RegionSectionCoords& right)
		{
			return left.hex < right.hex;
		}
	};

	/// 
	struct GlobalCoords
	{
		std::string region;
		RegionSectionCoords section;

		friend bool operator <(const GlobalCoords& left, const GlobalCoords& right)
		{
			return left.region < right.region || (left.region == right.region && left.section < right.section);
		}
	};
}

#endif
