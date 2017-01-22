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
	CorporealBeing::CorporealBeing(Game& game, const std::function<std::unique_ptr<Agent>(Being&)>& make_agent, BeingId id, Body body, const std::function<Stats()>& make_base_stats)
		: Being{game, make_agent, id, std::move(body), make_base_stats}
	{}

	CorporealBeing::CorporealBeing(Game& game, std::istream& in, Body body)
		: Being{game, in, std::move(body)}
	{}
}
