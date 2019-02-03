//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/stats/being.hpp"
#include "entities/beings/stats/modifier.hpp"
#include "utility/id.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace ql {
	struct being;

	enum class status_type { bane, neutral, boon };

	//! A status effect on a being, such as blindness or poison.
	struct status {
		//! The name of the status modifier displayed to the player.
		std::string name() const {
			return _name;
		}

		//! The number of ticks remaining before the status expires.
		tick duration() const {
			return _duration;
		}

		//! The ID of the being that caused the status or nullopt if none did.
		std::optional<id<being>> source() const {
			return _source_id;
		}

		//! The type of the status: bane, netural, or boon.
		virtual status_type type() const = 0;

		//! The list of stat modifiers associated with this status.
		virtual std::vector<stats::modifier> modifiers() const {
			// No modifiers by default.
			return {};
		}

		//! Carries out any effects that should occur when the status is initially applied.
		//! @param target The being affected by the status.
		virtual void apply(being& target);

		//! Carries out any effects that should occur each update cycle and decrements the duration of the status.
		//! @param target The being affected by the status.
		//! @param elapsed Time since the last update.
		void update(being& target, tick elapsed);

	protected:
		//! @param name The name of the status modifier displayed to the player.
		//! @param duration The number of ticks remaining before the status modifier expires.
		//! @param source The ID of the being that caused the status modifier, if any.
		status(std::string name, tick duration, std::optional<id<being>> source_id = std::nullopt);

	private:
		std::string _name;
		nonnegative<tick> _duration;
		std::optional<id<being>> _source_id;

		//! Carries out status subtype-specific effects that should occur each update cycle.
		//! @param target The being affected by the status.
		//! @param elapsed Time since the last update.
		virtual void subupdate(being& target, tick elapsed);

		//! Carries out any effects that should occur when the status expires.
		//! @param target The being affected by the status.
		virtual void expire(being& target);
	};
}
