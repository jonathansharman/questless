/**
* @file    CorporealBeing.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION A being with a physical body.
*/

#ifndef CORPOREAL_BEING_H
#define CORPOREAL_BEING_H

#include "Being.h"

namespace questless
{
	class CorporealBeing : public Being
	{
	public:
		bool corporeal() const override { return true; }
	protected:
		CorporealBeing(Game& game, const std::function<std::unique_ptr<Agent>(Being&)>& make_agent, BeingId id, Body body, const std::function<Attributes()>& make_base_attributes);
		CorporealBeing(Game& game, std::istream& in, Body body);
	};
}

#endif
