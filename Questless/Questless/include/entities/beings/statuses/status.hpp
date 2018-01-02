//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <optional>
#include <string>
#include <vector>
#include <memory>

#include "entities/beings/stats/stats.hpp"
#include "entities/beings/stats/modifier.hpp"
#include "utility/id.hpp"

namespace ql
{
	class being;

	enum class status_type { debuff, neutral, buff };

	//! A status effect on a being, such as blindness or poison.
	class status
	{
	public:
		//! The name of the status modifier displayed to the player.
		std::string name() const { return _name; }

		//! The number of turns remaining before the status expires.
		int duration() const { return _duration; }

		//! The ID of the being that caused the status or nullopt if none did.
		std::optional<id<being>> source() const { return _source_id; }

		//! The type of the status: debuff, netural, or buff.
		virtual status_type type() const = 0;

		//! The list of stat modifiers associated with this status.
		virtual std::vector<uptr<modifier>> const& modifiers() const
		{
			// No modifiers by default.
			static std::vector<uptr<modifier>> modifiers;
			return modifiers;
		}

		//! Carries out any effects that should occur when the status is initially applied.
		//! @param target The being affected by the status.
		virtual void apply(being& target);

		//! Carries out any effects that should occur each update cycle and decrements the duration of the status.
		//! @param target The being affected by the status.
		void update(being& target);
	protected:
		//! @param name The name of the status modifier displayed to the player.
		//! @param duration The number of turns remaining before the status modifier expires.
		//! @param source The ID of the being that caused the status modifier, if any.
		status(std::string name, int duration, std::optional<id<being>> source_id = std::nullopt);
	private:
		std::string _name;
		int _duration;
		std::optional<id<being>> _source_id;

		//! Carries out status subtype-specific effects that should occur each update cycle.
		virtual void subupdate(being& target);

		//! Carries out any effects that should occur when the status expires.
		//! @param target The being affected by the status.
		virtual void expire(being& target);
	};
}
