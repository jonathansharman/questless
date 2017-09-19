//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/weapons/Weapon.h"
#include "items/weapons/Quiver.h"

namespace questless
{
	class Bow : public WeaponBase<Bow>
	{
	public:
		Bow(Id<Item> id = Id<Item>::make())
			: Item{id}
			, WeaponBase<Bow>{durability()}
			, _fire{std::make_shared<Fire>(id)}
		{}

		std::string name() const final { return "Bow"; }

		double weight() const final { return 3.5; }

		double equip_time() const final { return 12.0; }
		double unequip_time() const final { return 3.0; }

		double durability() const final { return 400.0; }

		virtual std::vector<uptr<Action>> actions() final
		{
			std::vector<uptr<Action>> actions;
			if (equipped()) {
				actions.push_back(_fire->launch());
				actions.push_back(Unequip::make(*this));
			} else {
				actions.push_back(Equip::make(*this));
				actions.push_back(Drop::make(*this));
				actions.push_back(Throw::make(*this));
			}
			return actions;
		}
	private:
		class Fire : public RangedAttack
		{
		public:
			Fire(Id<Item> weapon_id) : RangedAttack{weapon_id} {}
			std::string name() const final { return "Fire"; }
			dmg::Group base_damage() const final { return dmg::Pierce{30.0}; }
			double wind_up() const final { return 5.0; }
			double follow_through() const final { return 0.5; }
			double cooldown() const final { return 1.0; }
			double wear_ratio() const final { return 0.002; };
			Cost const& cost() const final { return _cost; }
			int range() const final { return 7; }
		private:
			class ArrowCost : public Cost
			{
			public:
				Complete check(Being& actor, cont_t cont) const final;
				void incur(Being& actor) const final;
			};

			ArrowCost _cost;
		};

		sptr<Fire> _fire;

		Requirements requirements() const final { return Hands{2}; }
	};
}
