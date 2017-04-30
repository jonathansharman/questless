//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "utility/visitor-pattern.h"

namespace questless
{
	using ItemQuerySubtypeList = type_list::Empty;

	DEFINE_VISITORS(ItemQuery, ItemQuerySubtypeList)

	//! A request to an agent for an item from an inventory.
	struct ItemQuery : public Element<ItemQuerySubtypeList>
	{
		virtual ~ItemQuery() = default;
	};

	DEFINE_ELEMENT_BASE(ItemQuery, ItemQuery)
}
