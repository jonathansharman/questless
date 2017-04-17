//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/weapons/Weapon.h"
#include "items/weapons/Quiver.h"

namespace questless
{
	class Bow : public Weapon
	{
	public:
		Bow(Id<Item> id = Id<Item>::make())
			: Item{id}
			, Weapon{durability()}
			, _fire{std::make_shared<Fire>(id)}
		{}

		void accept(ItemVisitor& visitor) const override { visitor.visit(*this); }

		std::string name() const override { return "Bow"; }

		double weight() const override { return 3.5; }

		double equip_time() const override { return 12.0; }
		double unequip_time() const override { return 3.0; }

		double durability() const override { return 400.0; }

		virtual std::vector<Action::uptr> actions() override
		{
			std::vector<Action::uptr> actions;
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
			std::string name() const override { return "Fire"; }
			Damage base_damage() const override { return Pierce{30.0}; }
			double wind_up() const override { return 5.0; }
			double follow_through() const override { return 0.5; }
			double cooldown() const override { return 1.0; }
			double wear_ratio() const override { return 0.002; };
			Cost const& cost() const override { return _cost; }
			int range() const override { return 7; }
		private:
			class ArrowCost : public Cost
			{
			public:
				Complete check(Being& actor, cont_t cont) const override;
				void incur(Being& actor) const override;
			};

			ArrowCost _cost;
		};

		std::shared_ptr<Fire> _fire;

		Requirements requirements() const override { return Hands{2}; }
	};
}
