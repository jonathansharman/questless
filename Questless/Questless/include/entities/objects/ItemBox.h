/**
* @file    ItemBox.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION A box holding some items.
*/

#pragma once

#include <vector>

#include "Object.h"

namespace questless
{
	class ItemBox : public Object
	{
	public:
		ItemBox(Id<Object> id = Id<Object>::make()) : Object{id} {}
		ItemBox(std::istream& in) : Object{in} {}

		void accept(EntityVisitor& visitor) override { visitor.visit(*this); }
		void accept(EntityVisitor& visitor) const override { visitor.visit(*this); }

		virtual EntityClass entity_class() const { return EntityClass::ItemBoxClass; }

		std::vector<Id<Item>>& items() { return _items; }
		std::vector<Id<Item>> const& items() const { return _items; }
	private:
		std::vector<Id<Item>> _items;
	};
}
