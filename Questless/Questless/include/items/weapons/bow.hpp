//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/weapons/weapon.hpp"
#include "items/weapons/quiver.hpp"

namespace ql
{
	class bow : public weapon_base<bow>
	{
	public:
		bow(ql::id<item> id = ql::id<item>::make())
			: item{id}
			, weapon_base<bow>{durability()}
			, _shoot{std::make_shared<shoot>(id)}
		{}

		std::string name() const final { return "Bow"; }

		double weight() const final { return 3.5; }

		double equip_time() const final { return 12.0; }
		double unequip_time() const final { return 3.0; }

		double durability() const final { return 400.0; }

		virtual std::vector<uptr<action>> actions() final
		{
			std::vector<uptr<action>> actions;
			if (equipped()) {
				actions.push_back(_shoot->launch());
				actions.push_back(unequip::make(*this));
			} else {
				actions.push_back(equip::make(*this));
				actions.push_back(drop::make(*this));
				actions.push_back(toss::make(*this));
			}
			return actions;
		}
	private:
		class shoot : public ranged_attack
		{
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
			class arrow_cost : public cost
			{
			public:
				complete check(being& actor, cont cont) const final;
				void incur(being& actor) const final;
			};

			arrow_cost _cost;
		};

		sptr<shoot> _shoot;

		body_part_counts requirements() const final { return hands{2}; }
	};
}
