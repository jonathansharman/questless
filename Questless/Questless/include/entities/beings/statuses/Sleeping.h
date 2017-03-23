/**
* @file    Sleeping.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Sleeping status class.
*/

#pragma once

#include "entities/beings/statuses/Status.h"

namespace questless
{
	class Sleeping : public Status
	{
	public:
		/// @param magnitude How powerful the status modifier is.
		/// @param duration The number of turns remaining before the status modifier expires.
		/// @param source_id The ID of the being that caused the status modifier, if any.
		Sleeping(double magnitude, int duration, std::optional<Id<Being>> source_id = std::nullopt)
			: Status("Poisoned", duration, source_id), _magnitude{magnitude}
		{}

		virtual Type type() const { return Type::neutral; }

		void subupdate(Being& target) override;
	private:
		double _magnitude;
	};
}
