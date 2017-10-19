//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/item.hpp"
#include "magic/spell.hpp"
#include "utility/static_bounded.hpp"
#include "utility/dynamic_bounded.hpp"

namespace ql
{
	//! A gem that can hold spell charges.
	class gatestone : public item_base<gatestone>
	{
	public:
		constexpr gatestone(double capacity, double charge, double cooldown, magic::color color, ql::id<item> id = ql::id<item>::make())
			: item_base<gatestone>{id}
			, mana{charge, 0.0, capacity}
			, _capacity{capacity}
			, _cooldown{cooldown}
			, _color{color}
		{}

		std::string name() const final { return "Gatestone"; }

		double weight() const final { return 0.1; }

		std::vector<uptr<action>> actions() final;

		void update() final;

		//! The amount of mana this gem currently holds.
		dynamic_bounded<double> mana;

		//! The maximum charge this gem can hold.
		double capacity() const { return _capacity; }

		//! The color of spell with which this gem may be enchanted.
		magic::color color() const { return _color; }

		//! Time left before this gem can be used again.
		double cooldown() const { return _cooldown; }
	private:
		class charge : public action
		{
		public:
			charge(gatestone& gatestone) : _gatestone{gatestone} {}
			static auto make(gatestone& gatestone) { return std::make_unique<charge>(gatestone); }
			std::string name() const final { return "Charge"; }
			complete perform(being& actor, cont cont) final;
		private:
			gatestone& _gatestone;
		};

		class incant : public action
		{
		public:
			incant(gatestone& gatestone) : _gatestone{gatestone} {}

			static auto make(gatestone& gatestone) { return std::make_unique<incant>(gatestone); }

			std::string name() const final { return "Incant"; }

			complete perform(being& actor, cont cont) final;
		private:
			gatestone& _gatestone;
		};

		static constexpr double _minimum_cooldown = 0.0;

		double const _capacity;
		static_bounded<double, _minimum_cooldown> _cooldown;
		magic::color _color;
	};
}
