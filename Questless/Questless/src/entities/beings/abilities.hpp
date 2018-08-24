//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <vector>
#include <memory>

#include "agents/action.hpp"

namespace ql {
	class being;

	//! The list of actions a being is able to perform.
	class abilities {
	public:
		bool can_swim() const { return _can_swim; }
		bool can_fly() const { return _can_fly; }

		void update(being& being);
	private:
		bool _can_swim;
		bool _can_fly;
	};
}
