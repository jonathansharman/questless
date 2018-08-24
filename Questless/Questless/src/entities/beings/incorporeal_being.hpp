//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "being.hpp"

namespace ql {
	//! A being with no physical body.
	class incorporeal_being : public being {
	public:
		bool corporeal() const final { return false; }
	protected:
		incorporeal_being(const std::function<uptr<ql::agent>(being&)>& make_agent, ql::id<being> id, ql::body body, const std::function<ql::stats()>& make_base_stats);
		incorporeal_being(std::istream& in, ql::body body);
	};

	DEFINE_ELEMENT_BASE(incorporeal_being, entity)
}
