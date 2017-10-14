//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/corporeal_being.h"
#include "agents/agent.h"

namespace ql
{
	corporeal_being::corporeal_being(const std::function<uptr<ql::agent>(being&)>& make_agent, ql::id<being> id, ql::body body, const std::function<ql::stats()>& make_base_stats)
		: being{make_agent, id, std::move(body), make_base_stats}
	{}

	corporeal_being::corporeal_being(std::istream& in, ql::body body)
		: being{in, std::move(body)}
	{}
}
