//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "Being.h"

namespace questless
{
	//! A being with a physical body.
	class CorporealBeing : public Being
	{
	public:
		bool corporeal() const override { return true; }
	protected:
		CorporealBeing(const std::function<std::unique_ptr<Agent>(Being&)>& make_agent, Id<Being> id, Body body, const std::function<Stats()>& make_base_stats);
		CorporealBeing(std::istream& in, Body body);
	};

	DEFINE_ELEMENT_BASE(CorporealBeing, Entity)
}
