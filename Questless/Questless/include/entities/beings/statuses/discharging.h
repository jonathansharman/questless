//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/statuses/status.h"

namespace ql
{
	//! Causes a being to lose mana over time.
	class discharging : public status
	{
	public:
		//! @param magnitude How powerful the status modifier is.
		//! @param duration The number of turns remaining before the status modifier expires.
		//! @param source_id The ID of the being that caused the status modifier, if any.
		discharging(double magnitude, int duration, std::optional<id<being>> source_id = std::nullopt)
			: status("Discharging", duration, source_id), _magnitude{magnitude}
		{}

		virtual status_type type() const { return status_type::buff; }

		void subupdate(being& target) final;
	private:
		double _magnitude;
	};
}
