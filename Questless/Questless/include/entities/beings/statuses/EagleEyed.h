//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/statuses/Status.h"

namespace questless
{
	//! Increases visual acuity.
	class EagleEyed : public Status
	{
	public:
		//! @param magnitude How powerful the status modifier is.
		//! @param duration The number of turns remaining before the status modifier expires.
		//! @param source_id The ID of the being that caused the status modifier, if any.
		EagleEyed(double magnitude, int duration, std::optional<Id<Being>> source_id = std::nullopt)
			: Status("Blind", duration, source_id)
			, _magnitude{magnitude}
			, _modifiers{Modifier::make_vector(std::make_unique<VisualAcuityModifier>(magnitude))}
		{}

		virtual Type type() const { return Type::buff; }

		std::vector<Modifier::uptr> const& modifiers() const override { return _modifiers; }
	private:
		double _magnitude;
		std::vector<Modifier::uptr> _modifiers;
	};
}
