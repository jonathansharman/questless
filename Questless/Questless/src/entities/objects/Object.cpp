//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/objects/Object.h"
#include "Game.h"

namespace questless
{
	Object::Object(std::istream& in) : Entity{in}, id{in} {}

	void Object::serialize(std::ostream& out) const
	{
		Entity::serialize(out);

		out << id << ' ';

		//! @todo Object-specific serialization.
	}

	void Object::update()
	{
		//! @todo This.
	}
}
