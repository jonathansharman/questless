//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "object.hpp"

namespace ql {
	//! A simple source of heat and light.
	struct campfire : object_base<campfire> {
		campfire(ql::region& region, region_tile::point coords, ql::id<object> id = ql::id<object>::make());
		campfire(std::istream& in);

		virtual ql::entity_subtype entity_subtype() const {
			return entity_subtype::campfire;
		}

		double transparency() const final {
			return 1.0;
		}

		bool blocks_movement() const final {
			return true;
		}
	};
}
