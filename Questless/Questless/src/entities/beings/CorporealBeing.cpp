/**
* @file    CorporealBeing.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION CorporealBeing implementation.
*/

#include "entities/beings/CorporealBeing.h"
#include "agents/Agent.h"

namespace questless
{
	CorporealBeing::CorporealBeing(const std::function<std::unique_ptr<Agent>(Being&)>& make_agent, Id<Being> id, Body body, const std::function<Stats()>& make_base_stats)
		: Being{make_agent, id, std::move(body), make_base_stats}
	{}

	CorporealBeing::CorporealBeing(std::istream& in, Body body)
		: Being{in, std::move(body)}
	{}
}
