//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <vector>

#include "Object.h"

namespace questless
{
	//! A box capable of holding some items.
	class ItemBox : public ObjectBase<ItemBox>
	{
	public:
		ItemBox(Id<Object> id = Id<Object>::make()) : ObjectBase<ItemBox>{id} {}
		ItemBox(std::istream& in) : ObjectBase<ItemBox>{in} {}

		virtual EntityClass entity_class() const { return EntityClass::ItemBoxClass; }

		double transparency() const override { return 0.5; }

		bool blocks_movement() const override { return false; }

		std::vector<Id<Item>>& items() { return _items; }
		std::vector<Id<Item>> const& items() const { return _items; }
	private:
		std::vector<Id<Item>> _items;
	};
}
