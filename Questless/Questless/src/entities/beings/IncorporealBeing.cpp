//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/IncorporealBeing.h"
#include "agents/Agent.h"

namespace questless
{
	IncorporealBeing::IncorporealBeing(const std::function<uptr<Agent>(Being&)>& make_agent, Id<Being> id, Body body, const std::function<Stats()>& make_base_stats)
		: Being{make_agent, id, std::move(body), make_base_stats}
	{}

	IncorporealBeing::IncorporealBeing(std::istream& in, Body body)
		: Being{in, std::move(body)}
	{}
}
