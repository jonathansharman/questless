//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "Being.h"

namespace questless
{
	//! A being with no physical body.
	class IncorporealBeing : public Being
	{
	public:
		bool corporeal() const final { return false; }
	protected:
		IncorporealBeing(const std::function<uptr<Agent>(Being&)>& make_agent, Id<Being> id, Body body, const std::function<Stats()>& make_base_stats);
		IncorporealBeing(std::istream& in, Body body);
	};

	DEFINE_ELEMENT_BASE(IncorporealBeing, Entity)
}
