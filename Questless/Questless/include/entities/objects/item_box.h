//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <vector>

#include "object.h"

namespace ql
{
	//! A box capable of holding some items.
	class item_box : public object_base<item_box>
	{
	public:
		item_box(ql::id<object> id = ql::id<object>::make()) : object_base<item_box>{id} {}
		item_box(std::istream& in) : object_base<item_box>{in} {}

		virtual ql::entity_class entity_class() const { return entity_class::item_box_class; }

		double transparency() const final { return 0.5; }

		bool blocks_movement() const final { return false; }

		std::vector<ql::id<item>>& items() { return _items; }
		std::vector<ql::id<item>> const& items() const { return _items; }
	private:
		std::vector<ql::id<item>> _items;
	};
}
