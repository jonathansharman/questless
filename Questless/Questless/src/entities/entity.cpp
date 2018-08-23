//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/entity.hpp"
#include "world/region.hpp"
#include "world/section.hpp"

namespace ql {
	entity::entity(std::istream& in) : region {}, section{} {
		in >> coords.q;
		in >> coords.r;
		in >> coords.s;
	}

	void entity::serialize(std::ostream& out) const {
		out << static_cast<unsigned>(entity_subtype()) << ' ' << coords.q << ' ' << coords.r << ' ' << coords.s << ' ';
	}
}
