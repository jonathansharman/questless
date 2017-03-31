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
		using ref_less_t = bool(*)(Entity const&, Entity const&);
		using ptr_less_t = bool(*)(ptr const&, ptr const&);

		Region* region;
		Section* section;
		RegionTileCoords coords;

		virtual ~Entity() = default;

		/// Accepts an entity visitor. Used to implement the visitor pattern for entities.
		/// @param visitor An entity visitor.
		virtual void accept(EntityVisitor& visitor) = 0;
		////
		/// Accepts an entity visitor. Used to implement the visitor pattern for entities.
		/// @param visitor An entity visitor.
		////
		virtual void accept(EntityVisitor& visitor) const = 0;

		/// @return The entity's class's enumerated value.
		////
		virtual EntityClass entity_class() const = 0;

		/// @param out A stream object into which the serialized entity is inserted.
		////
		virtual void serialize(std::ostream& out) const;

		/// Advances the entity one turn.
		////
		virtual void update() = 0;
	protected:
		Entity() : region{}, section{}, coords{} {}
		Entity(std::istream& in);
	};
}
