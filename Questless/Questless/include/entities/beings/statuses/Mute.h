//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/statuses/Status.h"

namespace questless
{
	//! Causes muteness.
	class Mute : public Status
	{
	public:
		//! @param duration The number of turns remaining before the status modifier expires.
		//! @param source_id The ID of the being that caused the status modifier, if any.
		Mute(int duration, std::optional<Id<Being>> source_id = std::nullopt)
			: Status("Mute", duration, source_id), _modifiers{std::make_unique<MuteModifier>(true)}
		{}

		virtual Type type() const { return Type::debuff; }

		std::vector<Modifier::uptr> const& modifiers() const override { return _modifiers; }
	private:
		std::vector<Modifier::uptr> _modifiers;
	};
}
