//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/equipment.hpp"
#include "magic/spell.hpp"
#include "utility/static_bounded.hpp"
#include "utility/dynamic_bounded.hpp"

namespace ql
{
	//! A gem that can hold spell charges.
	class gatestone : public equipment_base<gatestone>
	{
	public:
		gatestone(double capacity, double charge, double cooldown, magic::color color, ql::id<item> id = ql::id<item>::make())
			: item{id}
			, mana{charge, 0.0, capacity}
			, _autocharge{true}
			, _capacity{capacity}
			, _cooldown{cooldown}
			, _color{color}
		{}

		std::string name() const final { return "Gatestone"; }

		double weight() const final { return 0.1; }

		std::vector<uptr<action>> actions() final;

		void update() final;

		//! The amount of time required to equip the item.
		double equip_time() const final { return 1.0; }

		//! The amount of time required to unequip the item.
		double unequip_time() const final { return 1.0; }

		//! The amount of mana this gem currently holds.
		dynamic_bounded<double> mana;

		//! The maximum charge this gem can hold.
		double capacity() const { return _capacity; }

		//! The color of spell with which this gem may be enchanted.
		magic::color color() const { return _color; }

		//! Time left before this gem can be used again.
		double cooldown() const { return _cooldown; }
	private:
		friend class toggle_autocharge;
		class toggle_autocharge : public action
		{
		public:
			toggle_autocharge(gatestone& gatestone, bool delay)
				: _gatestone_id{gatestone.id}
				, _enable_autocharge{!gatestone._autocharge}
				, _incur_delay{delay}
			{}
			static auto make(gatestone& gatestone, bool delay) { return umake<toggle_autocharge>(gatestone, delay); }
			std::string name() const final { return _enable_autocharge ? "Enable auto-charge" : "Disable auto-charge"; }
			complete perform(being& actor, cont cont) final;
		private:
			ql::id<item> _gatestone_id;
			bool _enable_autocharge;
			bool _incur_delay;
		};

		// Implementation needs access to auto-charge field.
		friend class complete_toggle;

		class incant : public action
		{
		public:
			incant(gatestone& gatestone) : _gatestone_id{gatestone.id} {}
			static auto make(gatestone& gatestone) { return umake<incant>(gatestone); }
			std::string name() const final { return "Incant"; }
			complete perform(being& actor, cont cont) final;
		private:
			ql::id<item> _gatestone_id;
		};

		static constexpr double _minimum_cooldown = 0.0;

		bool _autocharge;
		double const _capacity;
		static_bounded<double, _minimum_cooldown> _cooldown;
		magic::color _color;

		body_part_counts requirements() const final { return body_part_counts{hands{1}}; };
	};
}
