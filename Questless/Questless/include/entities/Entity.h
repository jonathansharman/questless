/**
* @file    Entity.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Entity class, which includes both beings and objects that can exist on the map.
*/

#pragma once

#include <string>
#include <iostream>
#include <memory>

#include "EntityVisitor.h"
#include "utility/utility.h"
#include "world/coordinates.h"

namespace questless
{
	enum class EntityClass : unsigned
		{ HumanClass
		, GoblinClass
		, TrollClass
		, CorpseClass
		, ItemBoxClass
		};
	class Game;
	class Region;
	class Section;

	class Entity
	{
	public:
		using ptr = std::unique_ptr<Entity>;
		using ref_less_t = bool(*)(const Entity&, const Entity&);
		using ptr_less_t = bool(*)(const ptr&, const ptr&);

		virtual ~Entity() = default;

		/// Accepts an entity visitor. Used to implement the visitor pattern for entities.
		/// @param visitor An entity visitor.
		virtual void accept(EntityVisitor& visitor) = 0;
		/// Accepts an entity visitor. Used to implement the visitor pattern for entities.
		/// @param visitor An entity visitor.
		virtual void accept(EntityVisitor& visitor) const = 0;

		/// @return The entity's class's enumerated value.
		virtual EntityClass entity_class() const = 0;

		/// @param out A stream object into which the serialized entity is inserted.
		virtual void serialize(std::ostream& out) const;

		/// Advances the entity one turn.
		virtual void update() = 0;

		/// @return The game in which the entity is taking part.
		Game& game() const { return _game; }

		/// @return The entity's region.
		Region& region() const { return *_region; }
		/// @param value The entity's new region.
		void region(Region* value) { _region = value; }

		/// @return The entity's section.
		Section& section() const { return *_section; }
		/// @param value The entity's new section.
		void section(Section* value) { _section = value; }

		/// @return The entity's coordinates within its region.
		RegionTileCoords coords() const { return _coords; }
		/// @param value The entity's new coordinates within its region.
		void coords(RegionTileCoords value) { _coords = value; }
	protected:
		Entity(Game& game) : _game{game}, _region{}, _section{}, _coords{} {}
		Entity(Game& game, std::istream& in);
	private:
		Game& _game;
		Region* _region;
		Section* _section;
		RegionTileCoords _coords;
	};
}
