/**
* @file    Healing.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface and implementation for the Healing status class.
*/

#pragma once

#include "entities/beings/statuses/Status.h"

namespace questless
{
	class Healing : public Status
	{
	public:
		/// @param magnitude How powerful the status modifier is.
		/// @param duration The number of turns remaining before the status modifier expires.
		/// @param source_id The ID of the being that caused the status modifier, if any.
		Healing(double magnitude, int duration, boost::optional<Id<Being>> source_id = boost::none)
			: Status("Healing", duration, source_id), _magnitude{magnitude}
		{}

		virtual Type type() const { return Type::buff; }

		void subupdate(Being& target) override;
	private:
		double _magnitude;
	};
}
