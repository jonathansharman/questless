/**
* @file    Confused.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface and implementation for the Confused status class.
*/

#pragma once

#include "entities/beings/statuses/Status.h"

namespace questless
{
	class Confused : public Status
	{
	public:
		/// @param magnitude How powerful the status modifier is.
		/// @param duration The number of turns remaining before the status modifier expires.
		/// @param source_id The ID of the being that caused the status modifier, if any.
		Confused(double magnitude, int duration, std::optional<Id<Being>> source_id = nullopt)
			: Status("Confused", duration, source_id)
			, _magnitude{magnitude}
			, _modifiers{std::make_unique<Stats::IntellectModifier>(-magnitude)}
		{}

		virtual Type type() const { return Type::debuff; }

		Stats::modifiers_t const& modifiers() const override { return _modifiers; }
	private:
		double _magnitude;
		Stats::modifiers_t _modifiers;
	};
}
