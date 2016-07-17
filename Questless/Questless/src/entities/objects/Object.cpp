/**
* @file    Object.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Object class.
*/

#include "entities/objects/Object.h"

namespace questless
{
	void Object::serialize(std::ostream& out) const
	{
		Entity::serialize(out);

		/// @todo Object-specific serialization.
	}

	void Object::update()
	{
		/// @todo This.
	}
}
