//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/statuses/Status.h"

namespace questless
{
	//! Causes a being to gain mana over time.
	class Recharging : public Status
	{
	public:
		//! @param magnitude How powerful the status modifier is.
		//! @param duration The number of turns remaining before the status modifier expires.
		//! @param source_id The ID of the being that caused the status modifier, if any.
		Recharging(double magnitude, int duration, std::optional<Id<Being>> source_id = std::nullopt)
			: Status("Recharging", duration, source_id), _magnitude{magnitude}
		{}

		virtual Type type() const { return Type::buff; }

		void subupdate(Being& target) final;
	private:
		double _magnitude;
	};
}
