//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/incorporeal_being.hpp"
#include "agents/agent.hpp"

namespace ql
{
	incorporeal_being::incorporeal_being(const std::function<uptr<ql::agent>(being&)>& make_agent, ql::id<being> id, ql::body body, const std::function<ql::stats()>& make_base_stats)
		: being{make_agent, id, std::move(body), make_base_stats}
	{}

	incorporeal_being::incorporeal_being(std::istream& in, ql::body body)
		: being{in, std::move(body)}
	{}
}
