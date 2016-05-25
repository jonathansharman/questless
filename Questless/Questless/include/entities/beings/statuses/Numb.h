/**
* @file    Numb.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface and implementation for the Numb status class.
*/

#ifndef NUMB_H
#define NUMB_H

#include "entities/beings/statuses/Status.h"

namespace questless
{
	class Numb : public Status
	{
	public:
		/// @param magnitude How powerful the status modifier is.
		/// @param duration The number of turns remaining before the status modifier expires.
		/// @param source The being that caused the status modifier, if any.
		Numb(double magnitude, unsigned duration, Being* source = nullptr)
			: Status{"Numb", duration, source}
			, _magnitude{magnitude}
			, _modifiers
				{ std::make_unique<Attributes::AgilityModifier>(-_magnitude)
				, std::make_unique<Attributes::DexterityModifier>(-_magnitude)
				}
		{}

		virtual Type type() const { return Type::debuff; }

		const modifiers_t& modifiers() const override { return _modifiers; }
	private:
		double _magnitude;
		modifiers_t _modifiers;
	};
}

#endif