/**
* @file    ItemBox.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION A box holding some items.
*/

#ifndef ITEM_BOX_H
#define ITEM_BOX_H

#include <vector>

#include "Object.h"

namespace questless
{
	class ItemBox : public Object
	{
	public:
		ItemBox(Game& game, ObjectId id) : Object(game, id) {}
		ItemBox(Game& game, std::istream& in) : Object(game, in) {}

		void accept(EntityVisitor& visitor) override { visitor.visit(*this); }
		void accept(EntityVisitor& visitor) const override { visitor.visit(*this); }

		virtual EntityClass entity_class() const { return EntityClass::ItemBoxClass; }

		std::vector<Item::ptr>& items() { return _items; }
		const std::vector<Item::ptr>& items() const { return _items; }
	private:
		std::vector<Item::ptr> _items;
	};
}

#endif
