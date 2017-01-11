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
	CorporealBeing::CorporealBeing(Game& game, const std::function<std::unique_ptr<Agent>(Being&)>& agent_factory, BeingId id, Body body, Attributes base_attributes)
		: Being{game, agent_factory, id, std::move(body), base_attributes}
	{}

	CorporealBeing::CorporealBeing(Game& game, std::istream& in, Body body)
		: Being{game, in, std::move(body)}
	{}
}
