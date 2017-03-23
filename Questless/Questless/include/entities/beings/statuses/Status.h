/**
* @file    Status.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Status class, which represents a status effect on a being.
*/

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

	class Status
	{
	public:
		enum class Type { debuff, neutral, buff };

		/// @return The name of the status modifier displayed to the player.
		std::string name() const { return _name; }

		/// @return The number of turns remaining before the status expires.
		int duration() const { return _duration; }

		/// @return The ID of the being that caused the status or nullopt if none did.
		std::optional<Id<Being>> source() const { return _source_id; }

		/// @return The type of the status: debuff, netural, or buff.
		virtual Type type() const = 0;

		/// @return The list of stat modifiers associated with this status.
		virtual std::vector<Modifier::ptr> const& modifiers() const
		{
			// No modifiers by default.
			static std::vector<Modifier::ptr> modifiers;
			return modifiers;
		}

		/// Carries out any effects that should occur when the status is initially applied.
		/// @param target The being affected by the status.
		virtual void apply(Being& target);

		/// Carries out any effects that should occur each update cycle and decrements the duration of the status.
		/// @param target The being affected by the status.
		void update(Being& target);

		/// Carries out any effects that should occur when the status expires.
		/// @param target The being affected by the status.
		virtual void expire(Being& target);
	protected:
		/// @param name The name of the status modifier displayed to the player.
		/// @param duration The number of turns remaining before the status modifier expires.
		/// @param source The ID of the being that caused the status modifier, if any.
		Status(std::string name, int duration, std::optional<Id<Being>> source_id = std::nullopt);
	private:
		std::string _name;
		int _duration;
		std::optional<Id<Being>> _source_id;

		virtual void subupdate(Being& target);
	};
}
