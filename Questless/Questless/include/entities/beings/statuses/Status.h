//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <vector>
#include <memory>

#include "entities/beings/stats/Stats.h"
#include "entities/beings/stats/Modifier.h"
#include "utility/Id.h"

namespace questless
{
	class Being;

	//! A status effect on a being, such as blindness or poison.
	class Status
	{
	public:
		enum class Type { debuff, neutral, buff };

		//! The name of the status modifier displayed to the player.
		std::string name() const { return _name; }

		//! The number of turns remaining before the status expires.
		int duration() const { return _duration; }

		//! The ID of the being that caused the status or nullopt if none did.
		std::optional<Id<Being>> source() const { return _source_id; }

		//! The type of the status: debuff, netural, or buff.
		virtual Type type() const = 0;

		//! The list of stat modifiers associated with this status.
		virtual std::vector<Modifier::uptr> const& modifiers() const
		{
			// No modifiers by default.
			static std::vector<Modifier::uptr> modifiers;
			return modifiers;
		}

		//! Carries out any effects that should occur when the status is initially applied.
		//! @param target The being affected by the status.
		virtual void apply(Being& target);

		//! Carries out any effects that should occur each update cycle and decrements the duration of the status.
		//! @param target The being affected by the status.
		void update(Being& target);

		//! Carries out any effects that should occur when the status expires.
		//! @param target The being affected by the status.
		virtual void expire(Being& target);
	protected:
		//! @param name The name of the status modifier displayed to the player.
		//! @param duration The number of turns remaining before the status modifier expires.
		//! @param source The ID of the being that caused the status modifier, if any.
		Status(std::string name, int duration, std::optional<Id<Being>> source_id = std::nullopt);
	private:
		std::string _name;
		int _duration;
		std::optional<Id<Being>> _source_id;

		//! Carries out Status subtype-specific effects that should occur each update cycle.
		virtual void subupdate(Being& target);
	};
}
