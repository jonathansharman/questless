/**
* @file
* @author Jonathan Sharman
* @copyright See LICENSE.txt.
*
*/

#pragma once

#include <memory>

namespace questless
{
	//! Visitor type for count queries.
	struct CountQueryVisitor
	{
		virtual ~CountQueryVisitor() = default;
	};

	//! A request to an agent for a count.
	struct CountQuery
	{
		using uptr = std::unique_ptr<CountQuery>;
		virtual ~CountQuery() = default;
		virtual void accept(CountQueryVisitor& visitor) = 0;
	};
}
