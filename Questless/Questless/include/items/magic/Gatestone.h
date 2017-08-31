//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/Item.h"
#include "spell/Spell.h"
#include "utility/Bounded.h"
#include "utility/DynamicBounded.h"

namespace questless
{
	//! A gem that can hold spell charges.
	class Gatestone : public ItemBase<Gatestone>
	{
	public:
		constexpr Gatestone(double capacity, double charge, double cooldown, spell::Color color, Id<Item> id = Id<Item>::make())
			: ItemBase<Gatestone>{id}
			, mana{charge, 0.0, capacity}
			, _capacity{capacity}
			, _cooldown{cooldown}
			, _color{color}
		{}

		std::string name() const final { return "Gatestone"; }

		double weight() const final { return 0.1; }

		std::vector<uptr<Action>> actions() final;

		void update() final;

		//! The amount of mana this gem currently holds.
		DynamicBounded<double> mana;

		//! The maximum charge this gem can hold.
		double capacity() const { return _capacity; }

		//! The color of spell with which this gem may be enchanted.
		spell::Color color() const { return _color; }

		//! Time left before this gem can be used again.
		double cooldown() const { return _cooldown; }
	private:
		class Charge : public Action
		{
		public:
			Charge(Gatestone& gatestone) : _gatestone{gatestone} {}
			static auto make(Gatestone& gatestone) { return std::make_unique<Charge>(gatestone); }
			std::string name() const final { return "Charge"; }
			Complete perform(Being& actor, cont_t cont) final;
		private:
			Gatestone& _gatestone;
		};

		class Incant : public Action
		{
		public:
			Incant(Gatestone& gatestone) : _gatestone{gatestone} {}

			static auto make(Gatestone& gatestone) { return std::make_unique<Incant>(gatestone); }

			std::string name() const final { return "Incant"; }

			Complete perform(Being& actor, cont_t cont) final;
		private:
			Gatestone& _gatestone;
		};

		static constexpr double _minimum_cooldown = 0.0;

		double const _capacity;
		Bounded<double, _minimum_cooldown> _cooldown;
		spell::Color _color;
	};
}
