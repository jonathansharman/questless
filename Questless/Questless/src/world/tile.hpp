//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "tile_visitor.hpp"

#include "quantities/quantities.hpp"

namespace ql {
	//! The basic unit of terrain, a hexagonal region of uniform material, temperature, etc.
	class tile : public element<tile_subtype_list> {
	public:
		ql::temperature temperature_offset; //!< Offset from ambient temperature at this tile.

		//! @param temperaure_offset Offset from ambient temperature at this tile.
		tile(ql::temperature temperature_offset) : temperature_offset{temperature_offset} {}

		virtual ~tile() {}

		//! The tile's type enumeration, for use in serialization and deserialization.
		virtual tile_subtype subtype() const = 0;
	};

	DEFINE_ELEMENT_BASE(tile, tile)

	class dirt_tile : public tile_base<dirt_tile> {
	public:
		using tile_base<dirt_tile>::tile_base;
		tile_subtype subtype() const final { return tile_subtype::dirt; }
	};

	class edge_tile : public tile_base<edge_tile> {
	public:
		using tile_base<edge_tile>::tile_base;
		tile_subtype subtype() const final { return tile_subtype::edge; }
	};

	class grass_tile : public tile_base<grass_tile> {
	public:
		using tile_base<grass_tile>::tile_base;
		tile_subtype subtype() const final { return tile_subtype::grass; }
	};

	class sand_tile : public tile_base<sand_tile> {
	public:
		using tile_base<sand_tile>::tile_base;
		tile_subtype subtype() const final { return tile_subtype::sand; }
	};

	class snow_tile : public tile_base<snow_tile> {
	public:
		using tile_base<snow_tile>::tile_base;
		tile_subtype subtype() const final { return tile_subtype::snow; }
	};

	class stone_tile : public tile_base<stone_tile> {
	public:
		using tile_base<stone_tile>::tile_base;
		tile_subtype subtype() const final { return tile_subtype::stone; }
	};

	class water_tile : public tile_base<water_tile> {
	public:
		using tile_base<water_tile>::tile_base;
		tile_subtype subtype() const final { return tile_subtype::water; }
	};
}
