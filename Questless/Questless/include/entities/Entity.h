//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <iostream>
#include <memory>

#include "EntityVisitor.h"
#include "utility/reference.h"
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

	//! Things that can exist on the world map, including beings and objects.
	class Entity : public Element<EntityMutableVisitor, EntityConstVisitor>
	{
	public:
		using ref_less_t = bool(*)(Entity const&, Entity const&);
		using ptr_less_t = bool(*)(uptr<Entity> const&, uptr<Entity> const&);

		Region* region;
		Section* section;
		RegionTileCoords coords;

		virtual ~Entity() = default;

		//! The entity's class's enumerated value.
		virtual EntityClass entity_class() const = 0;

		//! @param out A stream object into which the serialized entity is inserted.
		virtual void serialize(std::ostream& out) const;

		//! Advances the entity one turn.
		virtual void update() = 0;
	protected:
		Entity() : region{}, section{}, coords{} {}
		Entity(std::istream& in);
	};

	DEFINE_ELEMENT_BASE(Entity, Entity)
}
