//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/equipment.hpp"
#include "magic/color.hpp"
#include "quantities/mass.hpp"
#include "utility/nonnegative.hpp"
#include "utility/dynamic_bounded.hpp"

namespace ql {
	//! A gem that can hold spell charges.
	class gatestone : public equipment_base<gatestone> {
	public:
		gatestone(mana capacity, mana charge, tick cooldown, magic::color color, ql::id<item> id = ql::id<item>::make())
			: item{id}
			, charge{charge, 0.0_mp, capacity}
			, _capacity{capacity}
			, _cooldown{cooldown}
			, _color{color}
		{}

		std::string name() const final { return "Gatestone"; }

		load mass() const final { return 0.1_load; }

		std::vector<uptr<action>> actions() final;

		void update(tick elapsed) final;

		tick equip_time() const final { return 10_tick; }

		tick unequip_time() const final { return 10_tick; }

		//! The amount of mana this gatestone currently holds.
		dynamic_bounded<mana> charge;

		//! The maximum charge this gem can hold.
		mana capacity() const { return _capacity; }

		//! The color of spell with which this gem may be enchanted.
		magic::color color() const { return _color; }

		//! Time left before this gem can be used again.
		tick cooldown() const { return _cooldown; }
	private:
		class incant : public action {
		public:
			incant(gatestone& gatestone) : _gatestone_id{gatestone.id} {}
			static auto make(gatestone& gatestone) { return umake<incant>(gatestone); }
			std::string name() const final { return "Incant"; }
			complete perform(being& actor, cont cont) final;
		private:
			ql::id<item> _gatestone_id;
		};

		mana _capacity;
		nonnegative<tick> _cooldown;
		magic::color _color;

		body_part_counts requirements() const final { return body_part_counts{hands{1}}; };
	};
}
