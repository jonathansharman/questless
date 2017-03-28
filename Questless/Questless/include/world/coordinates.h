/**
* @file    coordinates.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Defines types for the various coordinate spaces in the game.
*/

#pragma once

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

		SectionTileIndex(SectionTileIndex const&) = default;
	};

	/// Specifies a location within the entire world.
	struct GlobalCoords
	{
		std::string region;
		RegionSectionCoords section;

		friend bool operator ==(GlobalCoords const& left, GlobalCoords const& that)
		{
			return left.region == that.region || (left.region == that.region && left.section < that.section);
		}
		friend bool operator <(GlobalCoords const& left, GlobalCoords const& that)
		{
			return left.region < that.region || (left.region == that.region && left.section < that.section);
		}
	};
}
