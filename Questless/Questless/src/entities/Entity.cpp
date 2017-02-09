/**
* @file    Entity.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Entity class.
*/

#include "entities/Entity.h"
#include "world/Region.h"
#include "world/Section.h"

namespace questless
{
	Entity::Entity(Game& game, std::istream& in) : game{game}, region {}, section{}
	{
		in >> coords.q;
		in >> coords.r;
		in >> coords.s;
	}

	void Entity::serialize(std::ostream& out) const
	{
		out << static_cast<unsigned>(entity_class()) << ' ' << coords.q << ' ' << coords.r << ' ' << coords.s << ' ';
	}
}
