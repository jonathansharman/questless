//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/Entity.h"
#include "world/Region.h"
#include "world/Section.h"

namespace questless
{
	Entity::Entity(std::istream& in) : region {}, section{}
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
