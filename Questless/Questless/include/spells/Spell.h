/**
* @file    Spell.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Spell abstract base class.
*/

#ifndef SPELL_H
#define SPELL_H

#include <string>
#include <functional>
#include <memory>
#include <stdexcept>

#include "utility/optional.h"

#include "entities/beings/Action.h"

namespace questless
{
	class Spell : public Action
	{
	public:
		enum class Color { white, black, green, red, blue, yellow };

		using ptr = std::unique_ptr<Spell>;

		virtual ~Spell() = default;

		std::string name() const override { return "Cast"; }

		/// @return The spell's color.
		virtual Color color() const = 0;

		/// @return The maximum number of charges the spell can hold or nullopt if the spell has infinite charges.
		virtual optional<int> max_charges() const = 0;

		/// @return The current number of spell charges.
		int charges() const{ return _charges; }

		/// Sets the number of spell charges to the given value.
		void charges(int value) { _charges = value; }

		/// Increases the number of charges by the given amount.
		/// @param amount The number of charges to add, 1 by default.
		void charge(int amount = 1);

		/// Decreases the number of charges by the given amount.
		/// @param amount The number of charges to subtract, 1 by default.
		void discharge(int amount = 1);

		/// @return Time after casting the spell before it can be used again.
		virtual double cooldown() const = 0;

		/// @return Time left before the spell can be cast again.
		double active_cooldown() const { return _active_cooldown; }

		/// Sets the time left before the spell can be cast again to the given value.
		void active_cooldown(double value) { _active_cooldown = value; }

		/// Advances the spell one time unit.
		virtual void update();
	protected:
		Spell(int charges = 0) : _charges{charges}, _active_cooldown{0} {}
	private:
		int _charges;
		double _active_cooldown;
	};
}

#endif