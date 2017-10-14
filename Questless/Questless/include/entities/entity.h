//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <iostream>
#include <memory>

#include "entity_visitor.h"
#include "utility/reference.h"
#include "utility/utility.h"
#include "world/coordinates.h"

namespace ql
{
	enum class entity_class : unsigned
		// Beings
		{ human_class
		, goblin_class
		, troll_class
		// Objects
		, campfire
		, corpse_class
		, item_box_class
		};
	class game;
	class region;
	class section;

	//! Things that can exist on the world map, including beings and objects.
	class entity : public element<entity_subtype_list>
	{
	public:
		using ref_less_t = bool(*)(entity const&, entity const&);
		using ptr_less_t = bool(*)(uptr<entity> const&, uptr<entity> const&);

		region* region;
		section* section;
		region_tile::point coords;

		virtual ~entity() = default;

		//! The entity's class's enumerated value.
		virtual entity_class entity_class() const = 0;

		//! @param out A stream object into which the serialized entity is inserted.
		virtual void serialize(std::ostream& out) const;

		//! Advances this entity one time unit.
		virtual void update() = 0;

		//! The proportion of light or visual information this entity allows through, in the range [0, 1].
		virtual double transparency() const = 0;
	protected:
		entity() : region{}, section{}, coords{} {}
		entity(std::istream& in);
	};

	DEFINE_ELEMENT_BASE(entity, entity)
}
