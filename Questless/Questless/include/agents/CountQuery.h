/**
* @file    CountQuery.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Types of count queries, i.e. requests to an agent for a count.
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
		using ptr = std::unique_ptr<CountQuery>;
		virtual ~CountQuery() = default;
		virtual void accept(CountQueryVisitor& visitor) = 0;
	};
}
