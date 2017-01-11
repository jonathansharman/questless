/**
* @file    IncorporealBeing.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION IncorporealBeing implementation.
*/

#include "entities/beings/IncorporealBeing.h"
#include "agents/Agent.h"

namespace questless
{
	IncorporealBeing::IncorporealBeing(Game& game, const std::function<std::unique_ptr<Agent>(Being&)>& agent_factory, BeingId id, Body body, Attributes base_attributes)
		: Being{game, agent_factory, id, std::move(body), base_attributes}
	{}

	IncorporealBeing::IncorporealBeing(Game& game, std::istream& in, Body body)
		: Being{game, in, std::move(body)}
	{}
}
