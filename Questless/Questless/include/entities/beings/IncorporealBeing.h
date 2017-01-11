/**
* @file    IncorporealBeing.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION A being with no physical body.
*/

#ifndef INCORPOREAL_BEING_H
#define INCORPOREAL_BEING_H

#include "Being.h"

namespace questless
{
	class IncorporealBeing : public Being
	{
	public:
		bool corporeal() const override { return true; }
	protected:
		IncorporealBeing(Game& game, const std::function<std::unique_ptr<Agent>(Being&)>& agent_factory, BeingId id, Body body, Attributes base_attributes);
		IncorporealBeing(Game& game, std::istream& in, Body body);
	};
}

#endif
