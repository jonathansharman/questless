/**
* @file    Object.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Object class.
*/

#include "entities/objects/Object.h"
#include "Game.h"

namespace questless
{
	Object::Object(Game& game, std::istream& in) : Entity(game, in), id{in} {}

	void Object::serialize(std::ostream& out) const
	{
		Entity::serialize(out);

		out << id << ' ';

		/// @todo Object-specific serialization.
	}

	void Object::update()
	{
		/// @todo This.
	}
}
