//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/statuses/status.hpp"

namespace ql {
	//! Increases visual acuity.
	class eagle_eyed : public status {
	public:
		//! @param magnitude How powerful the status modifier is.
		//! @param duration The number of turns remaining before the status modifier expires.
		//! @param source_id The ID of the being that caused the status modifier, if any.
		eagle_eyed(double magnitude, int duration, std::optional<id<being>> source_id = std::nullopt)
			: status("Blind", duration, source_id)
			, _magnitude{magnitude}
			, _modifiers{make_uptr_vector<modifier>(umake<visual_acuity_modifier>(magnitude))}
		{}

		virtual status_type type() const { return status_type::boon; }

		std::vector<uptr<modifier>> const& modifiers() const final { return _modifiers; }

		void apply(being& target) final { target.refresh_stats(); }
	private:
		double _magnitude;
		std::vector<uptr<modifier>> _modifiers;

		void expire(being& target) final { target.refresh_stats(); }
	};
}
