//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Defines types for the various coordinate spaces in the game.

#pragma once

#include "units/hex_coords.hpp"

namespace ql
{
	//! The space of tiles in a section.
	using section_tile = units::hex_space<struct section_tile_tag>;

	//! The space of tiles in a region.
	using region_tile = units::hex_space<struct region_tile_tag>;

	//! The space of sections in a region.
	using region_section = units::hex_space<struct region_section_tag>;

	//! Location within the entire world.
	struct global_coords
	{
		std::string region;
		region_section::point section;

		friend bool operator ==(global_coords const& left, global_coords const& right)
		{
			return left.region == right.region || (left.region == right.region && left.section < right.section);
		}
		friend bool operator <(global_coords const& left, global_coords const& right)
		{
			return left.region < right.region || (left.region == right.region && left.section < right.section);
		}
	};
}

// Specialize std::hash.
namespace std
{
	template <>
	struct hash<ql::section_tile::point>
	{
		size_t operator()(ql::section_tile::point const& p) const
		{
			return hash_value(p);
		}
	};
}
namespace std
{
	template <>
	struct hash<ql::section_tile::vector>
	{
		size_t operator()(ql::section_tile::vector const& v) const
		{
			return hash_value(v);
		}
	};
}
namespace std
{
	template <>
	struct hash<ql::region_tile::point>
	{
		size_t operator()(ql::region_tile::point const& p) const
		{
			return hash_value(p);
		}
	};
}
namespace std
{
	template <>
	struct hash<ql::region_tile::vector>
	{
		size_t operator()(ql::region_tile::vector const& v) const
		{
			return hash_value(v);
		}
	};
}
namespace std
{
	template <>
	struct hash<ql::region_section::point>
	{
		size_t operator()(ql::region_section::point const& p) const
		{
			return hash_value(p);
		}
	};
}
namespace std
{
	template <>
	struct hash<ql::region_section::vector>
	{
		size_t operator()(ql::region_section::vector const& v) const
		{
			return hash_value(v);
		}
	};
}
