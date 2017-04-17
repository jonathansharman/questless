//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines types for the various coordinate spaces in the game.

#pragma once

#include "units/HexCoords.h"

namespace questless
{
	//! Hexagonal coordinates of a tile within a section.
	using SectionTileCoords = units::HexCoords<struct SectionTileCoordsTag>;

	//! Hexagonal coordinates of a tile within a region.
	using RegionTileCoords = units::HexCoords<struct RegionTileCoordsTag>;

	//! Hexagonal coordinates of a section within a region.
	using RegionSectionCoords = units::HexCoords<struct RegionSectionCoordsTag>;

	//! Location within the entire world.
	struct GlobalCoords
	{
		std::string region;
		RegionSectionCoords section;

		friend bool operator ==(GlobalCoords const& left, GlobalCoords const& right)
		{
			return left.region == right.region || (left.region == right.region && left.section < right.section);
		}
		friend bool operator <(GlobalCoords const& left, GlobalCoords const& right)
		{
			return left.region < right.region || (left.region == right.region && left.section < right.section);
		}
	};
}
