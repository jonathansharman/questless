//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/weapons/weapon.hpp"

namespace ql {
	class bow : public weapon_base<bow> {
	public:
		bow(ql::id<item> id = ql::id<item>::make())
			: item{id}
			, weapon_base<bow>{durability()}
			, _shoot{smake<shoot>(id)}
		{}

		std::string name() const final { return "Bow"; }

		double weight() const final { return 3.5; }

		double equip_time() const final { return 12.0; }
		double unequip_time() const final { return 3.0; }

		double durability() const final { return 400.0; }

		std::vector<uptr<action>> actions() final;
	private:
		class shoot : public ranged_attack {
		public:
			shoot(ql::id<item> weapon_id) : ranged_attack{weapon_id} {}
			std::string name() const final { return "Shoot"; }
			dmg::group base_damage() const final { return dmg::pierce{30.0}; }
			double wind_up() const final { return 5.0; }
			double follow_through() const final { return 0.5; }
			double cooldown() const final { return 1.0; }
			double wear_ratio() const final { return 0.002; };
			ql::cost const& cost() const final { return _cost; }
			int range() const final { return 7; }
		private:
			class arrow_cost : public cost {
			public:
				complete check(being& actor, cont cont) const final;
				void incur(being& actor) const final;
			};

			arrow_cost _cost;

			sptr<effect> get_effect(region_tile::point source, region_tile::point target) final;
		};

		sptr<shoot> _shoot;

		body_part_counts requirements() const final { return hands{2}; }
	};
}
