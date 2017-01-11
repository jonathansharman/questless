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
#include <memory>

#include <boost/optional.hpp>

#include "entities/beings/Action.h"

namespace questless::spell
{
	enum class Color { white, black, green, red, blue, yellow };

	class Spell
	{
	public:
		using ptr = std::unique_ptr<Spell>;

		class Cast : public Action
		{
		public:
			Cast(Spell& spell) : _spell{spell} {}
			static ptr make(Spell& spell) { return std::make_unique<Cast>(spell); }
			std::string name() const override { return "Cast"; }
			Action::Complete perform(Being& actor, cont_t cont) override;
		private:
			Spell& _spell;
		};

		class Incant : public Action
		{
		public:
			Incant(Spell& spell) : _spell{spell} {}
			static ptr make(Spell& spell) { return std::make_unique<Incant>(spell); }
			std::string name() const override { return "Incant"; }
			Action::Complete perform(Being& actor, cont_t cont) override;
		private:
			Spell& _spell;
		};

		class Discharge : public Action
		{
		public:
			Discharge(Spell& spell) : _spell{spell} {}
			static ptr make(Spell& spell) { return std::make_unique<Discharge>(spell); }
			std::string name() const override { return "Discharge"; }
			Action::Complete perform(Being& actor, cont_t cont) override;
		private:
			Spell& _spell;
		};

		/// @return An action that casts the spell.
		Action::ptr cast() { return Cast::make(*this); }

		/// @return An action that charges the spell for one charge.
		Action::ptr incant() { return Incant::make(*this); }

		/// @return An action that discharges the spell for one charge.
		Action::ptr discharge() { return Discharge::make(*this); }

		virtual ~Spell() = default;

		/// @return The spell's color.
		virtual Color color() const = 0;

		/// @return The maximum number of charges the spell can hold or nullopt if the spell has infinite charges.
		virtual boost::optional<int> max_charges() const = 0;

		/// @return The current number of spell charges.
		int charges() const{ return _charges; }

		/// Sets the number of spell charges to the given value.
		void charges(int value) { _charges = value; }

		/// Increases the number of charges by the given amount.
		/// @param amount The number of charges to add, 1 by default.
		void gain_charge(int amount);

		/// Decreases the number of charges by the given amount.
		/// @param amount The number of charges to subtract, 1 by default.
		void lose_charge(int amount);

		/// @return The amount of time between starting to cast the spell and the completion of the cast.
		virtual double cast_time() const = 0;

		/// @return The amount of time required to incant the spell.
		virtual double incant_time() const = 0;

		/// @return The amount of time required to incant the spell.
		virtual double discharge_time() const = 0;

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

		virtual Action::Complete perform_cast(Being& actor, Action::cont_t cont) = 0;
	};
}

#endif
