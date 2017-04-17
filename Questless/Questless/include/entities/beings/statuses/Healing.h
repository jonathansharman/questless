//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/statuses/Status.h"

namespace questless
{
	//! A status that causes a being to gain health over time.
	class Healing : public Status
	{
	public:
		//! @param magnitude How powerful the status modifier is.
		//! @param duration The number of turns remaining before the status modifier expires.
		//! @param source_id The ID of the being that caused the status modifier, if any.
		Healing(double magnitude, int duration, std::optional<Id<Being>> source_id = std::nullopt)
			: Status("Healing", duration, source_id), _magnitude{magnitude}
		{}

		virtual Type type() const { return Type::buff; }

		void subupdate(Being& target) override;
	private:
		double _magnitude;
	};
}
