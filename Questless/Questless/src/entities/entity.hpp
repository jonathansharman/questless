//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <iostream>
#include <memory>

#include "entity_visitor.hpp"
#include "utility/reference.hpp"
#include "world/coordinates.hpp"

namespace ql {
	class game;
	class region;
	class section;

	//! Things that can exist on the world map, including beings and objects.
	class entity : public element<entity_subtype_list> {
	public:
		using ref_less_t = bool(*)(entity const&, entity const&);
		using ptr_less_t = bool(*)(uptr<entity> const&, uptr<entity> const&);

		region* region;
		section* section;
		region_tile::point coords;

		virtual ~entity() = default;

		//! The entity's class's enumerated value.
		virtual entity_subtype entity_subtype() const = 0;

		//! Advances this entity in time by @elapsed.
		virtual void update(tick elapsed) = 0;

		//! The proportion of light or visual information this entity allows through, in the range [0, 1].
		virtual double transparency() const = 0;
	protected:
		entity() : region{}, section{}, coords{} {}
	};

	DEFINE_ELEMENT_BASE(entity, entity)
}
