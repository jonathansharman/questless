/**
* @file    IncorporealBeing.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION A being with no physical body.
*/

#pragma once

#include "Being.h"

namespace questless
{
	class IncorporealBeing : public Being
	{
	public:
		bool corporeal() const override { return true; }
	protected:
		IncorporealBeing(const std::function<std::unique_ptr<Agent>(Being&)>& make_agent, Id<Being> id, Body body, const std::function<Stats()>& make_base_stats);
		IncorporealBeing(std::istream& in, Body body);
	};
}
