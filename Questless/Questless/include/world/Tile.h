//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "sdl/Renderer.h"
#include "sdl/Texture.h"
#include "utility/visitor-pattern.h"

namespace questless
{
	class DirtTile;
	class EdgeTile;
	class GrassTile;
	class SandTile;
	class SnowTile;
	class StoneTile;
	class WaterTile;

	using TileSubtypeList = type_list::of_t
		< DirtTile
		, EdgeTile
		, GrassTile
		, SandTile
		, SnowTile
		, StoneTile
		, WaterTile
		>;

	DEFINE_VISITORS(Tile, TileSubtypeList)

	//! The basic unit of terrain, a hexagonal region of uniform material, temperature, etc.
	class Tile : public Element<TileSubtypeList>
	{
	public:
		enum class TileClass : int { dirt = 0, edge, grass, sand, snow, stone, water, TILE_CLASS_COUNT }; //! @todo Remove TILE_CLASS_COUNT if static reflection that can check enum length is ever added.

		double temperature_offset; //!< Offset from ambient temperature at this tile.

		//! @param temperaure_offset Offset from ambient temperature at this tile.
		Tile(double temperature_offset) : temperature_offset{temperature_offset} {}

		virtual ~Tile() {}

		//! The tile's type enumeration, for use in serialization and deserialization.
		virtual TileClass tile_class() const = 0;
	};

	DEFINE_ELEMENT_BASE(Tile, Tile)

	class DirtTile : public TileBase<DirtTile>
	{
	public:
		using TileBase<DirtTile>::TileBase;
		TileClass tile_class() const override { return TileClass::dirt; }
	};

	class EdgeTile : public TileBase<EdgeTile>
	{
	public:
		using TileBase<EdgeTile>::TileBase;
		TileClass tile_class() const override { return TileClass::edge; }
	};

	class GrassTile : public TileBase<GrassTile>
	{
	public:
		using TileBase<GrassTile>::TileBase;
		TileClass tile_class() const override { return TileClass::grass; }
	};

	class SandTile : public TileBase<SandTile>
	{
	public:
		using TileBase<SandTile>::TileBase;
		TileClass tile_class() const override { return TileClass::sand; }
	};

	class SnowTile : public TileBase<SnowTile>
	{
	public:
		using TileBase<SnowTile>::TileBase;
		TileClass tile_class() const override { return TileClass::snow; }
	};

	class StoneTile : public TileBase<StoneTile>
	{
	public:
		using TileBase<StoneTile>::TileBase;
		TileClass tile_class() const override { return TileClass::stone; }
	};

	class WaterTile : public TileBase<WaterTile>
	{
	public:
		using TileBase<WaterTile>::TileBase;
		TileClass tile_class() const override { return TileClass::water; }
	};
}
