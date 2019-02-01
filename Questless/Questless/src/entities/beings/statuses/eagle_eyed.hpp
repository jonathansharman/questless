//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/statuses/status.hpp"

namespace ql {
	//! Increases visual acuity.
	class telescoped : public status {
	public:
		//! @param magnitude How powerful the status modifier is.
		//! @param duration The number of turns remaining before the status modifier expires.
		//! @param source_id The ID of the being that caused the status modifier, if any.
		telescoped(acuity magnitude, tick duration, std::optional<id<being>> source_id = std::nullopt)
			: status{"Blind", duration, source_id}
			, _magnitude{magnitude}
		{}

		virtual status_type type() const { return status_type::boon; }

		std::vector<stats::modifier> modifiers() const final { return {stats::acuity_modifier{_magnitude}}; }

		void apply(being& target) final { target.refresh_stats(); }
	private:
		acuity _magnitude;

		void expire(being& target) final { target.refresh_stats(); }
	};
}
