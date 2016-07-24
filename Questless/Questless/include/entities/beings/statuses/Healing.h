/**
* @file    Healing.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface and implementation for the Healing status class.
*/

#ifndef HEALING_H
#define HEALING_H

#include "entities/beings/statuses/Status.h"

namespace questless
{
	class Healing : public Status
	{
	public:
		/// @param magnitude How powerful the status modifier is.
		/// @param duration The number of turns remaining before the status modifier expires.
		/// @param source_id The ID of the being that caused the status modifier, if any.
		Healing(double magnitude, unsigned duration, optional<BeingId> source_id = nullopt)
			: Status("Healing", duration, source_id), _magnitude{magnitude}
		{}

		virtual Type type() const { return Type::buff; }

		void subupdate(Being& target) override;
	private:
		double _magnitude;
	};
}

#endif
