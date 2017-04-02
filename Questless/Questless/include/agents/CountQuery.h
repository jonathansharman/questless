/**
* @file    CountQuery.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Requests to an agent for a count.
*/

#pragma once

#include <memory>

namespace questless
{
	struct CountQueryVisitor
	{
		virtual ~CountQueryVisitor() = default;
	};

	struct CountQuery
	{
		using uptr = std::unique_ptr<CountQuery>;
		virtual ~CountQuery() = default;
		virtual void accept(CountQueryVisitor& visitor) = 0;
	};
}
