/**
* @file    Blind.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface and implementation for the Blind status class.
*/

#pragma once

#include "entities/beings/statuses/Status.h"

namespace questless
{
	class Blind : public Status
	{
	public:
		/// @param magnitude How powerful the status modifier is.
		/// @param duration The number of turns remaining before the status modifier expires.
		/// @param source_id The ID of the being that caused the status modifier, if any.
		Blind(double magnitude, int duration, std::optional<Id<Being>> source_id = nullopt)
			: Status("Blind", duration, source_id)
			, _magnitude{magnitude}
			, _modifiers{std::make_unique<Stats::VisionModifier>(-magnitude)}
		{}

		virtual Type type() const { return Type::debuff; }

		Stats::modifiers_t const& modifiers() const override { return _modifiers; }
	private:
		double _magnitude;
		Stats::modifiers_t _modifiers;
	};
}
