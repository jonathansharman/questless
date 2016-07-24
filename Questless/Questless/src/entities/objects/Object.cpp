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
	Object::Object(Game& game, std::istream& in) : Entity(game, in)
	{
		ObjectId::key_t id_key;
		in >> id_key;
		_id = id_key;
	}

	void Object::serialize(std::ostream& out) const
	{
		Entity::serialize(out);

		out << id().key << ' ';

		/// @todo Object-specific serialization.
	}

	void Object::update()
	{
		/// @todo This.
	}
}
