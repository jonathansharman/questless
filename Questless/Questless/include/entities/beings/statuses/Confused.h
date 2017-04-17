//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/statuses/Status.h"

namespace questless
{
	//! An intellect-reducing status.
	class Confused : public Status
	{
	public:
		//! @param magnitude How powerful the status modifier is.
		//! @param duration The number of turns remaining before the status modifier expires.
		//! @param source_id The ID of the being that caused the status modifier, if any.
		Confused(double magnitude, int duration, std::optional<Id<Being>> source_id = std::nullopt)
			: Status("Confused", duration, source_id)
			, _magnitude{magnitude}
			, _modifiers{std::make_unique<IntellectModifier>(-magnitude)}
		{}

		virtual Type type() const { return Type::debuff; }

		std::vector<Modifier::uptr> const& modifiers() const override { return _modifiers; }
	private:
		double _magnitude;
		std::vector<Modifier::uptr> _modifiers;
	};
}
