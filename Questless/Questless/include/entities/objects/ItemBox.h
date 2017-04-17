//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <vector>

#include "Object.h"

namespace questless
{
	//! A box capable of holding some items.
	class ItemBox : public Object
	{
	public:
		ItemBox(Id<Object> id = Id<Object>::make()) : Object{id} {}
		ItemBox(std::istream& in) : Object{in} {}

		void accept(EntityVisitor& visitor) override { visitor.visit(*this); }
		void accept(EntityVisitor& visitor) const override { visitor.visit(*this); }

		virtual EntityClass entity_class() const { return EntityClass::ItemBoxClass; }

		bool blocks_movement() const override { return false; }

		std::vector<Id<Item>>& items() { return _items; }
		std::vector<Id<Item>> const& items() const { return _items; }
	private:
		std::vector<Id<Item>> _items;
	};
}
