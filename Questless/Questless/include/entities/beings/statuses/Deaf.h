/**
* @file    Deaf.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface and implementation for the Numb status class.
*/

#ifndef DEAF_H
#define DEAF_H

#include "entities/beings/statuses/Status.h"

namespace questless
{
	class Deaf : public Status
	{
	public:
		/// @param magnitude How powerful the status modifier is.
		/// @param duration The number of turns remaining before the status modifier expires.
		/// @param source_id The ID of the being that caused the status modifier, if any.
		Deaf(double magnitude, unsigned duration, boost::optional<BeingId> source_id = boost::none)
			: Status("Deaf", duration, source_id)
			, _magnitude{magnitude}
			, _modifiers{std::make_unique<Attributes::IntellectModifier>(-magnitude)}
		{}

		virtual Type type() const { return Type::debuff; }

		const Attributes::modifiers_t& modifiers() const override { return _modifiers; }
	private:
		double _magnitude;
		Attributes::modifiers_t _modifiers;
	};
}

#endif
