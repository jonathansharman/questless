//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/statuses/status.hpp"

namespace ql {
	//! Causes a being to sleep.
	class sleeping : public status {
	public:
		//! @param magnitude How powerful the status modifier is.
		//! @param duration The number of turns remaining before the status modifier expires.
		//! @param source_id The ID of the being that caused the status modifier, if any.
		sleeping(double magnitude, int duration, std::optional<id<being>> source_id = std::nullopt)
			: status("Poisoned", duration, source_id), _magnitude{magnitude}
		{}

		virtual status_type type() const { return status_type::neutral; }

		void subupdate(being& target) final;
	private:
		double _magnitude;
	};
}
