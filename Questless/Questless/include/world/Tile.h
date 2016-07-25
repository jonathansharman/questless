/**
* @file    Tile.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Tile class.
*/

#ifndef TILE_H
#define TILE_H

#include "sdl-wrappers/Renderer.h"
#include "sdl-wrappers/Texture.h"

namespace questless
{
	class EdgeTile;
	class StoneTile;
	class DirtTile;
	class GrassTile;
	class WaterTile;
	class SnowTile;

	class TileVisitor
	{
	public:
		virtual ~TileVisitor() = default;

		virtual void visit(const EdgeTile&) = 0;
		virtual void visit(const StoneTile&) = 0;
		virtual void visit(const DirtTile&) = 0;
		virtual void visit(const GrassTile&) = 0;
		virtual void visit(const WaterTile&) = 0;
		virtual void visit(const SnowTile&) = 0;
	};

	class BadTileVisit : public std::runtime_error
	{
	public:
		BadTileVisit(std::string message) : std::runtime_error{std::move(message)} {}
	};

	class TrivialTileVisitor : public TileVisitor
	{
	public:
		TrivialTileVisitor(std::string message) : _message{std::move(message)} {}

		virtual ~TrivialTileVisitor() = default;

		virtual void visit(const EdgeTile&) override { throw BadTileVisit{_message}; }
		virtual void visit(const StoneTile&) override { throw BadTileVisit{_message}; }
		virtual void visit(const DirtTile&) override { throw BadTileVisit{_message}; }
		virtual void visit(const GrassTile&) override { throw BadTileVisit{_message}; }
		virtual void visit(const WaterTile&) override { throw BadTileVisit{_message}; }
		virtual void visit(const SnowTile&) override { throw BadTileVisit{_message}; }
	private:
		std::string _message;
	};

	class Tile
	{
	public:
		enum class TileClass : int {edge = 0, stone, dirt, grass, water, snow};

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

	class EdgeTile : public Tile
	{
	public:
		using Tile::Tile;
		TileClass tile_class() const override { return TileClass::edge; }
		void accept(TileVisitor& visitor) const override { visitor.visit(*this); }
	};

	class StoneTile : public Tile
	{
	public:
		using Tile::Tile;
		TileClass tile_class() const override { return TileClass::stone; }
		void accept(TileVisitor& visitor) const override { visitor.visit(*this); }
	};

	class DirtTile : public Tile
	{
	public:
		using Tile::Tile;
		TileClass tile_class() const override { return TileClass::dirt; }
		void accept(TileVisitor& visitor) const override { visitor.visit(*this); }
	};

	class GrassTile : public Tile
	{
	public:
		using Tile::Tile;
		TileClass tile_class() const override { return TileClass::grass; }
		void accept(TileVisitor& visitor) const override { visitor.visit(*this); }
	};

	class WaterTile : public Tile
	{
	public:
		using Tile::Tile;
		TileClass tile_class() const override { return TileClass::water; }
		void accept(TileVisitor& visitor) const override { visitor.visit(*this); }
	};

	class SnowTile : public Tile
	{
	public:
		using Tile::Tile;
		TileClass tile_class() const override { return TileClass::snow; }
		void accept(TileVisitor& visitor) const override { visitor.visit(*this); }
	};
}

#endif