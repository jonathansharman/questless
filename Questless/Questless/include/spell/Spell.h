//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <optional>

#include "agents/Action.h"
#include "SpellVisitor.h"
#include "utility/reference.h"

namespace questless::spell
{
	enum class Color { white, black, green, red, blue, yellow };

	//! A magical spell that can be cast by a being.
	class Spell : public Element<SpellSubtypeList>
	{
	public:
		class Cast : public Action
		{
		public:
			Cast(Spell& spell) : _spell{spell} {}
			static auto make(Spell& spell) { return std::make_unique<Cast>(spell); }
			std::string name() const final { return "Cast"; }
			Complete perform(Being& actor, cont_t cont) final;
		private:
			Spell& _spell;
		};

		class Incant : public Action
		{
		public:
			Incant(Spell& spell) : _spell{spell} {}
			static auto make(Spell& spell) { return std::make_unique<Incant>(spell); }
			std::string name() const final { return "Incant"; }
			Complete perform(Being& actor, cont_t cont) final;
		private:
			Spell& _spell;
		};

		class Discharge : public Action
		{
		public:
			Discharge(Spell& spell) : _spell{spell} {}
			static auto make(Spell& spell) { return std::make_unique<Discharge>(spell); }
			std::string name() const final { return "Discharge"; }
			Complete perform(Being& actor, cont_t cont) final;
		private:
			Spell& _spell;
		};

		virtual ~Spell() = default;

		//! An action that casts the spell.
		uptr<Action> cast() { return Cast::make(*this); }

		//! An action that charges the spell for one charge.
		uptr<Action> incant() { return Incant::make(*this); }

		//! An action that discharges the spell for one charge.
		uptr<Action> discharge() { return Discharge::make(*this); }

		//! The spell's name.
		virtual std::string name() const = 0;

		//! The spell's color.
		virtual Color color() const = 0;

		//! The maximum number of charges the spell can hold or nullopt if the spell has infinite charges.
		virtual std::optional<int> max_charges() const = 0;

		//! The current number of spell charges.
		int charges() const{ return _charges; }

		//! Sets the number of spell charges to the given value.
		void charges(int value) { _charges = value; }

		//! Increases the number of charges by the given amount.
		//! @param amount The number of charges to add, 1 by default.
		void gain_charge(int amount);

		//! Decreases the number of charges by the given amount.
		//! @param amount The number of charges to subtract, 1 by default.
		void lose_charge(int amount);

		//! The amount of time between starting to cast the spell and the completion of the cast.
		virtual double cast_time() const = 0;

		//! The amount of time required to incant the spell.
		virtual double incant_time() const = 0;

		//! The amount of time required to incant the spell.
		virtual double discharge_time() const = 0;

		//! Time after casting the spell before it can be used again.
		virtual double cooldown() const = 0;

		//! Time left before the spell can be cast again.
		double active_cooldown() const { return _active_cooldown; }

		//! Sets the time left before the spell can be cast again to the given value.
		void active_cooldown(double value) { _active_cooldown = value; }

		//! Advances the spell one time unit.
		virtual void update();
	protected:
		Spell(int charges = 0) : _charges{charges}, _active_cooldown{0} {}
	private:
		int _charges;
		double _active_cooldown;

		virtual Complete perform_cast(Being& actor, Action::cont_t cont) = 0;
	};

	DEFINE_ELEMENT_BASE(Spell, Spell)
}
