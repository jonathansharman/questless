//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/statuses/status.hpp"

namespace ql {
	//! Deals blight damage over time.
	class poisoned : public status {
	public:
		//! @param magnitude How powerful the status modifier is.
		//! @param duration The number of turns remaining before the status modifier expires.
		//! @param source_id The ID of the being that caused the status modifier, if any.
		poisoned(double magnitude, tick duration, std::optional<id<being>> source_id = std::nullopt)
			: status("Poisoned", duration, source_id), _magnitude{magnitude}
		{}

		virtual status_type type() const { return status_type::bane; }

		void subupdate(being& target) final;
	private:
		double _magnitude;
	};
}
