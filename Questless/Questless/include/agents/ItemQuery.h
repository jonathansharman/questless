/**
* @file    ItemQuery.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Types of item queries, i.e. requests to an agent to choose an item from an inventory.
*/

#pragma once

#include <memory>

namespace questless
{
	struct ItemQueryVisitor
	{
		virtual ~ItemQueryVisitor() = default;
	};

	struct ItemQuery
	{
		using ptr = std::unique_ptr<ItemQuery>;
		virtual ~ItemQuery() = default;
		virtual void accept(ItemQueryVisitor& visitor) = 0;
	};
}
