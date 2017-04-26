//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>

namespace questless
{
	//! Visitor type for item queries.
	struct ItemQueryVisitor
	{
		virtual ~ItemQueryVisitor() = default;
	};

	//! A request to an agent for an item from an inventory.
	struct ItemQuery
	{
		virtual ~ItemQuery() = default;
		virtual void accept(ItemQueryVisitor& visitor) = 0;
	};
}
