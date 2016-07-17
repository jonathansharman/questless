/**
* @file    ItemBox.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the ItemBox class.
*/

#ifndef ITEM_BOX_H
#define ITEM_BOX_H

#include "Object.h"

namespace questless
{
	class ItemBox : public Object
	{
	public:
		ItemBox(id_t id) : Object(id) {}
		ItemBox(std::istream& in) : Object(in) {}

		void accept(EntityVisitor& visitor) override { visitor.visit(*this); }

		virtual EntityClass entity_class() const { return EntityClass::ItemBoxClass; }
	private:
		/// @todo Contained items
	};
}

#endif