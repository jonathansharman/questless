//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "sdl/Renderer.h"
#include "sdl/Texture.h"
#include "utility/Visitor.h"

namespace questless
{
	class DirtTile;
	class EdgeTile;
	class GrassTile;
	class SandTile;
	class SnowTile;
	class StoneTile;
	class WaterTile;

	using TileConstVisitor = Visitor
		< DirtTile const
		, EdgeTile const
		, GrassTile const
		, SandTile const
		, SnowTile const
		, StoneTile const
		, WaterTile const
		>;

	//! The basic unit of terrain, a hexagonal region of uniform material, temperature, etc.
	class Tile : public ConstElement<TileConstVisitor>
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

	DEFINE_CONST_ELEMENT_BASE(Tile, Tile)

	class DirtTile : public TileConstBase<DirtTile>
	{
	public:
		using TileConstBase<DirtTile>::TileConstBase;
		TileClass tile_class() const override { return TileClass::dirt; }
	};

	class EdgeTile : public TileConstBase<EdgeTile>
	{
	public:
		using TileConstBase<EdgeTile>::TileConstBase;
		TileClass tile_class() const override { return TileClass::edge; }
	};

	class GrassTile : public TileConstBase<GrassTile>
	{
	public:
		using TileConstBase<GrassTile>::TileConstBase;
		TileClass tile_class() const override { return TileClass::grass; }
	};

	class SandTile : public TileConstBase<SandTile>
	{
	public:
		using TileConstBase<SandTile>::TileConstBase;
		TileClass tile_class() const override { return TileClass::sand; }
	};

	class SnowTile : public TileConstBase<SnowTile>
	{
	public:
		using TileConstBase<SnowTile>::TileConstBase;
		TileClass tile_class() const override { return TileClass::snow; }
	};

	class StoneTile : public TileConstBase<StoneTile>
	{
	public:
		using TileConstBase<StoneTile>::TileConstBase;
		TileClass tile_class() const override { return TileClass::stone; }
	};

	class WaterTile : public TileConstBase<WaterTile>
	{
	public:
		using TileConstBase<WaterTile>::TileConstBase;
		TileClass tile_class() const override { return TileClass::water; }
	};
}
