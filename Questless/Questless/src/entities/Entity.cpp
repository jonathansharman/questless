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
	Entity::Entity(std::istream& in)
	{
		in >> _id;
		in >> _coords.hex.q;
		in >> _coords.hex.r;
		in >> _coords.hex.s;
	}

	void Entity::serialize(std::ostream& out) const
	{
		out << static_cast<unsigned>(entity_class()) << ' ' << entity_id() << ' ' << _coords.hex.q << ' ' << _coords.hex.r << ' ' << _coords.hex.s << ' ';
	}
}
