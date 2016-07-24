/**
* @file    Mute.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface and implementation for the Mute status class.
*/

#ifndef MUTE_H
#define MUTE_H

#include "entities/beings/statuses/Status.h"

namespace questless
{
	class Mute : public Status
	{
	public:
		/// @param duration The number of turns remaining before the status modifier expires.
		/// @param source_id The ID of the being that caused the status modifier, if any.
		Mute(unsigned duration, optional<BeingId> source_id = nullopt)
			: Status("Mute", duration, source_id), _modifiers{std::make_unique<Attributes::MuteModifier>(true)}
		{}

		virtual Type type() const { return Type::debuff; }

		const modifiers_t& modifiers() const override { return _modifiers; }
	private:
		modifiers_t _modifiers;
	};
}

#endif
