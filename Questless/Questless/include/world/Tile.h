/**
* @file    Tile.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Tile class.
*/

#pragma once

#include "sdl/Renderer.h"
#include "sdl/Texture.h"

namespace questless
{
	class DirtTile;
	class EdgeTile;
	class GrassTile;
	class SandTile;
	class SnowTile;
	class StoneTile;
	class WaterTile;

	class TileVisitor
	{
	public:
		virtual ~TileVisitor() = default;

		virtual void visit(DirtTile const&) = 0;
		virtual void visit(EdgeTile const&) = 0;
		virtual void visit(GrassTile const&) = 0;
		virtual void visit(SandTile const&) = 0;
		virtual void visit(SnowTile const&) = 0;
		virtual void visit(StoneTile const&) = 0;
		virtual void visit(WaterTile const&) = 0;
	};

	class Tile
	{
	public:
		enum class TileClass : int { dirt = 0, edge, grass, sand, snow, stone, water, TILE_CLASS_COUNT }; /// @todo Remove TILE_CLASS_COUNT if static reflection that can check enum length is ever added.

		Tile(double light_level, double temperature) : _light_level{light_level}, _temperature{temperature} {}

		virtual ~Tile() {}

		/// @return The tile's type enumeration, for use in serialization and deserialization.
		virtual TileClass tile_class() const = 0;

		virtual void accept(TileVisitor& visitor) const = 0;

		double light_level() const { return _light_level; }
		double temperature() const { return _temperature; }
	private:
		double _light_level;
		double _temperature;
	};

	class DirtTile : public Tile
	{
	public:
		using Tile::Tile;
		TileClass tile_class() const override { return TileClass::dirt; }
		void accept(TileVisitor& visitor) const override { visitor.visit(*this); }
	};

	class EdgeTile : public Tile
	{
	public:
		using Tile::Tile;
		TileClass tile_class() const override { return TileClass::edge; }
		void accept(TileVisitor& visitor) const override { visitor.visit(*this); }
	};

	class GrassTile : public Tile
	{
	public:
		using Tile::Tile;
		TileClass tile_class() const override { return TileClass::grass; }
		void accept(TileVisitor& visitor) const override { visitor.visit(*this); }
	};

	class SandTile : public Tile
	{
	public:
		using Tile::Tile;
		TileClass tile_class() const override { return TileClass::sand; }
		void accept(TileVisitor& visitor) const override { visitor.visit(*this); }
	};

	class SnowTile : public Tile
	{
	public:
		using Tile::Tile;
		TileClass tile_class() const override { return TileClass::snow; }
		void accept(TileVisitor& visitor) const override { visitor.visit(*this); }
	};

	class StoneTile : public Tile
	{
	public:
		using Tile::Tile;
		TileClass tile_class() const override { return TileClass::stone; }
		void accept(TileVisitor& visitor) const override { visitor.visit(*this); }
	};

	class WaterTile : public Tile
	{
	public:
		using Tile::Tile;
		TileClass tile_class() const override { return TileClass::water; }
		void accept(TileVisitor& visitor) const override { visitor.visit(*this); }
	};
}
