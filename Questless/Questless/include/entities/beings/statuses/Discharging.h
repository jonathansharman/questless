/**
* @file    Discharging.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Discharging status class.
*/

#ifndef DISCHARGING_H
#define DISCHARGING_H

#include "entities/beings/statuses/Status.h"

namespace questless
{
	class Discharging : public Status
	{
	public:
		/// @param magnitude How powerful the status modifier is.
		/// @param duration The number of turns remaining before the status modifier expires.
		/// @param source The being that caused the status modifier, if any.
		Discharging(double magnitude, unsigned duration, Being* source = nullptr) : Status{"Poisoned", duration, source}, _magnitude{magnitude} {}

		virtual Type type() const { return Type::buff; }

		void update(Being& target) override;
	private:
		double _magnitude;
	};
}

#endif