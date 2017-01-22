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
		IncorporealBeing(Game& game, const std::function<std::unique_ptr<Agent>(Being&)>& make_agent, BeingId id, Body body, const std::function<Stats()>& make_base_stats);
		IncorporealBeing(Game& game, std::istream& in, Body body);
	};
}

#endif
