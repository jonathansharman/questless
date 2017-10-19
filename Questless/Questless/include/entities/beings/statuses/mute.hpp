//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/statuses/status.hpp"

namespace ql
{
	//! Causes muteness.
	class mute : public status
	{
	public:
		//! @param duration The number of turns remaining before the status modifier expires.
		//! @param source_id The ID of the being that caused the status modifier, if any.
		mute(int duration, std::optional<id<being>> source_id = std::nullopt)
			: status("Mute", duration, source_id), _modifiers{std::make_unique<mute_modifier>(true)}
		{}

		virtual type type() const { return type::debuff; }

		std::vector<uptr<modifier>> const& modifiers() const final { return _modifiers; }
	private:
		std::vector<uptr<modifier>> _modifiers;
	};
}
