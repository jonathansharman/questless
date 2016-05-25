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
		/// @param source The being that caused the status modifier, if any.
		Deaf(double magnitude, unsigned duration, Being* source = nullptr)
			: Status{"Deaf", duration, source}
			, _magnitude{magnitude}
			, _modifiers{std::make_unique<Attributes::IntellectModifier>(-magnitude)}
		{}

		virtual Type type() const { return Type::debuff; }

		const modifiers_t& modifiers() const override
		{
			static vector<unique_ptr<Attributes::Modifier>> modifier{make_unique<Attributes::HearingModifier>(-_magnitude)};
			return modifier;
		}
	private:
		double _magnitude;
		modifiers_t _modifiers;
	};
}

#endif