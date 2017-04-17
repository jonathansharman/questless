//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/statuses/Status.h"

namespace questless
{
	//! A status representing that the being is sleeping.
	class Sleeping : public Status
	{
	public:
		//! @param magnitude How powerful the status modifier is.
		//! @param duration The number of turns remaining before the status modifier expires.
		//! @param source_id The ID of the being that caused the status modifier, if any.
		Sleeping(double magnitude, int duration, std::optional<Id<Being>> source_id = std::nullopt)
			: Status("Poisoned", duration, source_id), _magnitude{magnitude}
		{}

		virtual Type type() const { return Type::neutral; }

		void subupdate(Being& target) override;
	private:
		double _magnitude;
	};
}
