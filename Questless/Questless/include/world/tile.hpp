//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "sdl/renderer.hpp"
#include "sdl/texture.hpp"
#include "utility/visitor_pattern.hpp"

namespace ql
{
	class dirt_tile;
	class edge_tile;
	class grass_tile;
	class sand_tile;
	class snow_tile;
	class stone_tile;
	class water_tile;

	using tile_subtype_list = type_list::of_t
		< dirt_tile
		, edge_tile
		, grass_tile
		, sand_tile
		, snow_tile
		, stone_tile
		, water_tile
		>;

	DEFINE_VISITORS(tile, tile_subtype_list)


	enum class tile_subtype : int
		{ dirt = 0
		, edge
		, grass
		, sand
		, snow
		, stone
		, water
		, TILE_CLASS_COUNT //! @todo Remove TILE_CLASS_COUNT if static reflection that can check enum length is ever added.
		};

	//! The basic unit of terrain, a hexagonal region of uniform material, temperature, etc.
	class tile : public element<tile_subtype_list>
	{
	public:
		double temperature_offset; //!< Offset from ambient temperature at this tile.

		//! @param temperaure_offset Offset from ambient temperature at this tile.
		tile(double temperature_offset) : temperature_offset{temperature_offset} {}

		virtual ~tile() {}

		//! The tile's type enumeration, for use in serialization and deserialization.
		virtual tile_subtype subtype() const = 0;
	};

	DEFINE_ELEMENT_BASE(tile, tile)

	class dirt_tile : public tile_base<dirt_tile>
	{
	public:
		using tile_base<dirt_tile>::tile_base;
		tile_subtype subtype() const final { return tile_subtype::dirt; }
	};

	class edge_tile : public tile_base<edge_tile>
	{
	public:
		using tile_base<edge_tile>::tile_base;
		tile_subtype subtype() const final { return tile_subtype::edge; }
	};

	class grass_tile : public tile_base<grass_tile>
	{
	public:
		using tile_base<grass_tile>::tile_base;
		tile_subtype subtype() const final { return tile_subtype::grass; }
	};

	class sand_tile : public tile_base<sand_tile>
	{
	public:
		using tile_base<sand_tile>::tile_base;
		tile_subtype subtype() const final { return tile_subtype::sand; }
	};

	class snow_tile : public tile_base<snow_tile>
	{
	public:
		using tile_base<snow_tile>::tile_base;
		tile_subtype subtype() const final { return tile_subtype::snow; }
	};

	class stone_tile : public tile_base<stone_tile>
	{
	public:
		using tile_base<stone_tile>::tile_base;
		tile_subtype subtype() const final { return tile_subtype::stone; }
	};

	class water_tile : public tile_base<water_tile>
	{
	public:
		using tile_base<water_tile>::tile_base;
		tile_subtype subtype() const final { return tile_subtype::water; }
	};
}
