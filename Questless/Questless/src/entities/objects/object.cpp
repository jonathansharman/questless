//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/objects/object.h"
#include "game.h"

namespace ql
{
	object::object(std::istream& in) : entity{in}, id{in} {}

	void object::serialize(std::ostream& out) const
	{
		entity::serialize(out);

		out << id << ' ';

		//! @todo object-specific serialization.
	}

	void object::update()
	{
		//! @todo This.
	}
}
