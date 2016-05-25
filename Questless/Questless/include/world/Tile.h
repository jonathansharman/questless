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

#include <vector>
using std::vector;
#include <memory>
using std::unique_ptr;
using std::make_unique;
using std::shared_ptr;

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

	class BadTileVisit : public runtime_error
	{
	public:
		BadTileVisit(string message) : runtime_error{move(message)} {}
	};

	class TrivialTileVisitor : public TileVisitor
	{
	public:
		TrivialTileVisitor(string message) : _message{move(message)} {}

		virtual ~TrivialTileVisitor() = default;

		virtual void visit(const EdgeTile&) override { throw BadTileVisit{_message}; }
		virtual void visit(const StoneTile&) override { throw BadTileVisit{_message}; }
		virtual void visit(const DirtTile&) override { throw BadTileVisit{_message}; }
		virtual void visit(const GrassTile&) override { throw BadTileVisit{_message}; }
		virtual void visit(const WaterTile&) override { throw BadTileVisit{_message}; }
		virtual void visit(const SnowTile&) override { throw BadTileVisit{_message}; }
	private:
		string _message;
	};

	class Tile
	{
	public:
		enum class Type {edge, stone, dirt, grass, water, snow};

		virtual ~Tile() {}

		/// @return The tile's type enumeration, for use in serialization and deserialization.
		virtual Type type() const = 0;

		virtual void accept(TileVisitor& visitor) const = 0;
	};

	class EdgeTile : public Tile
	{
	public:
		Type type() const override { return Type::edge; }
		void accept(TileVisitor& visitor) const override { visitor.visit(*this); }
	};

	class StoneTile : public Tile
	{
	public:
		Type type() const override { return Type::stone; }
		void accept(TileVisitor& visitor) const override { visitor.visit(*this); }
	};

	class DirtTile : public Tile
	{
	public:
		Type type() const override { return Type::dirt; }
		void accept(TileVisitor& visitor) const override { visitor.visit(*this); }
	};

	class GrassTile : public Tile
	{
	public:
		Type type() const override { return Type::grass; }
		void accept(TileVisitor& visitor) const override { visitor.visit(*this); }
	};

	class WaterTile : public Tile
	{
	public:
		Type type() const override { return Type::water; }
		void accept(TileVisitor& visitor) const override { visitor.visit(*this); }
	};

	class SnowTile : public Tile
	{
	public:
		Type type() const override { return Type::snow; }
		void accept(TileVisitor& visitor) const override { visitor.visit(*this); }
	};
}

#endif