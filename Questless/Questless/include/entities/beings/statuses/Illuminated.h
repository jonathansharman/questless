/**
* @file    Illuminated.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface and implementation for the Illuminated status class.
*/

#pragma once

#include "entities/beings/statuses/Status.h"
#include "entities/beings/stats/Modifier.h"

namespace questless
{
	class Illuminated : public Status
	{
	public:
		/// @param magnitude How powerful the status modifier is.
		/// @param duration The number of turns remaining before the status modifier expires.
		/// @param source_id The ID of the being that caused the status modifier, if any.
		Illuminated(double magnitude, int duration, boost::optional<BeingId> source_id = boost::none)
			: Status("Illuminated", duration, source_id)
			, _magnitude{magnitude}
			, _modifiers
				{ std::make_unique<StealthModifier>(-_magnitude)
				, std::make_unique<BlackMagicModifier>(-_magnitude)
				}
		{}

		virtual Type type() const { return Type::debuff; }

		std::vector<Modifier::ptr> const& modifiers() const override { return _modifiers; }
	private:
		double _magnitude;
		std::vector<Modifier::ptr> _modifiers;
	};
}
