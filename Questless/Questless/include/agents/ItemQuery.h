/**
* @file    ItemQuery.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Requests to an agent for an item from an inventory.
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
		using uptr = std::unique_ptr<ItemQuery>;
		virtual ~ItemQuery() = default;
		virtual void accept(ItemQueryVisitor& visitor) = 0;
	};
}
