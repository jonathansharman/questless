/**
* @file    Illuminated.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface and implementation for the Illuminated status class.
*/

#ifndef ILLUMINATED_H
#define ILLUMINATED_H

#include "entities/beings/statuses/Status.h"

namespace questless
{
	class Illuminated : public Status
	{
	public:
		/// @param magnitude How powerful the status modifier is.
		/// @param duration The number of turns remaining before the status modifier expires.
		/// @param source_id The ID of the being that caused the status modifier, if any.
		Illuminated(double magnitude, unsigned duration, optional<BeingId> source_id = boost::none)
			: Status("Illuminated", duration, source_id)
			, _magnitude{magnitude}
			, _modifiers
				{ std::make_unique<Attributes::StealthModifier>(-_magnitude)
				, std::make_unique<Attributes::BlackPowerModifier>(-_magnitude)
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