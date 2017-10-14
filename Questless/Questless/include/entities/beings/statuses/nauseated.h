//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/statuses/status.h"

namespace ql
{
	//! Reduces strength and endurance.
	class nauseated : public status
	{
	public:
		//! @param magnitude How powerful the status modifier is.
		//! @param duration The number of turns remaining before the status modifier expires.
		//! @param source_id The ID of the being that caused the status modifier, if any.
		nauseated(double magnitude, int duration, std::optional<id<being>> source_id = std::nullopt)
			: status("Nauseated", duration, source_id)
			, _magnitude{magnitude}
			, _modifiers
				{ std::make_unique<strength_modifier>(-_magnitude)
				, std::make_unique<endurance_modifier>(-_magnitude)
				}
		{}

		virtual status_type type() const { return status_type::debuff; }

		std::vector<uptr<modifier>> const& modifiers() const final { return _modifiers; }
	private:
		double _magnitude;
		std::vector<uptr<modifier>> _modifiers;
	};
}
