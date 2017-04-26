//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/statuses/Status.h"

namespace questless
{
	//! Reduces strength and endurance.
	class Nauseated : public Status
	{
	public:
		//! @param magnitude How powerful the status modifier is.
		//! @param duration The number of turns remaining before the status modifier expires.
		//! @param source_id The ID of the being that caused the status modifier, if any.
		Nauseated(double magnitude, int duration, std::optional<Id<Being>> source_id = std::nullopt)
			: Status("Nauseated", duration, source_id)
			, _magnitude{magnitude}
			, _modifiers
				{ std::make_unique<StrengthModifier>(-_magnitude)
				, std::make_unique<EnduranceModifier>(-_magnitude)
				}
		{}

		virtual Type type() const { return Type::debuff; }

		std::vector<uptr<Modifier>> const& modifiers() const override { return _modifiers; }
	private:
		double _magnitude;
		std::vector<uptr<Modifier>> _modifiers;
	};
}
