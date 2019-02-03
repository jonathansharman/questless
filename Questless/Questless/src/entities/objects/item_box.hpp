//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <vector>

#include "object.hpp"

namespace ql {
	//! A box capable of holding some items.
	struct item_box : object_base<item_box> {
		item_box(ql::id<object> id = ql::id<object>::make()) : object_base<item_box>{id} {}

		virtual ql::entity_subtype entity_subtype() const {
			return entity_subtype::item_box_class;
		}

		double transparency() const final {
			return 0.5;
		}

		bool blocks_movement() const final {
			return false;
		}

		std::vector<ql::id<item>>& items() {
			return _items;
		}
		std::vector<ql::id<item>> const& items() const {
			return _items;
		}

	private:
		std::vector<ql::id<item>> _items;
	};
}
