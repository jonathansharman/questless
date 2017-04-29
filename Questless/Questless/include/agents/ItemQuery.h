//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/Visitor.h"

namespace questless
{
	using ItemQueryMutableVisitor = Visitor<>;
	using ItemQueryConstVisitor = Visitor<>;

	//! A request to an agent for an item from an inventory.
	struct ItemQuery : public Element<ItemQueryMutableVisitor, ItemQueryConstVisitor>
	{
		virtual ~ItemQuery() = default;
	};

	DEFINE_ELEMENT_BASE(ItemQuery, ItemQuery)
}
