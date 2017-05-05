//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/statuses/Status.h"

namespace questless
{
	//! Decreases visual acuity.
	class Blind : public Status
	{
	public:
		//! @param magnitude How powerful the status modifier is.
		//! @param duration The number of turns remaining before the status modifier expires.
		//! @param source_id The ID of the being that caused the status modifier, if any.
		Blind(double magnitude, int duration, std::optional<Id<Being>> source_id = std::nullopt)
			: Status("Blind", duration, source_id)
			, _magnitude{magnitude}
			, _modifiers{std::make_unique<VisualAcuityModifier>(-magnitude)}
		{}

		virtual Type type() const { return Type::debuff; }

		std::vector<uptr<Modifier>> const& modifiers() const final { return _modifiers; }
	private:
		double _magnitude;
		std::vector<uptr<Modifier>> _modifiers;
	};
}
