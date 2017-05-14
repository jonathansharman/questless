//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines types for the various coordinate spaces in the game.

#pragma once

#include "units/HexCoords.h"

namespace questless
{
	//! The space of tiles in a section.
	using SectionTile = units::HexSpace<struct SectionTileTag>;

	//! The space of tiles in a region.
	using RegionTile = units::HexSpace<struct RegionTileTag>;

	//! The space of sections in a region.
	using RegionSection = units::HexSpace<struct RegionSectionTag>;

	//! Location within the entire world.
	struct GlobalCoords
	{
		std::string region;
		RegionSection::Point section;

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

// Specialize std::hash.
namespace std
{
	template <>
	struct hash<questless::SectionTile::Point>
	{
		size_t operator()(questless::SectionTile::Point const& p) const
		{
			return hash_value(p);
		}
	};
}
namespace std
{
	template <>
	struct hash<questless::SectionTile::Vector>
	{
		size_t operator()(questless::SectionTile::Vector const& v) const
		{
			return hash_value(v);
		}
	};
}
namespace std
{
	template <>
	struct hash<questless::RegionTile::Point>
	{
		size_t operator()(questless::RegionTile::Point const& p) const
		{
			return hash_value(p);
		}
	};
}
namespace std
{
	template <>
	struct hash<questless::RegionTile::Vector>
	{
		size_t operator()(questless::RegionTile::Vector const& v) const
		{
			return hash_value(v);
		}
	};
}
namespace std
{
	template <>
	struct hash<questless::RegionSection::Point>
	{
		size_t operator()(questless::RegionSection::Point const& p) const
		{
			return hash_value(p);
		}
	};
}
namespace std
{
	template <>
	struct hash<questless::RegionSection::Vector>
	{
		size_t operator()(questless::RegionSection::Vector const& v) const
		{
			return hash_value(v);
		}
	};
}
