/**
* @file    Bow.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*/

#pragma once

#include "items/weapons/Weapon.h"

/// @todo Implementation file: Bow.cpp.

namespace questless
{
	class Bow : public Weapon
	{
	public:
		Bow(Id<Item> id = Id<Item>::make())
			: Item{id}
			, Weapon{durability()}
			, _fire{std::make_unique<Fire>(*this)}
		{}

		void accept(ItemVisitor& visitor) const override { visitor.visit(*this); }

		std::string name() const override { return "Bow"; }

		double weight() const override { return 3.5; }

		double equip_time() const override { return 12.0; }
		double unequip_time() const override { return 3.0; }

		double durability() const override { return 400.0; }

		virtual std::vector<Action::ptr> actions() override
		{
			std::vector<Action::ptr> actions;
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
			using RangedAttack::RangedAttack;
			std::string name() const override { return "Fire"; }
			virtual Damage base_damage() const override { return Pierce{30.0}; }
			virtual double wind_up() const override { return 5.0; }
			virtual double follow_through() const override { return 0.5; }
			virtual double cooldown() const override { return 1.0; }
			virtual double wear_ratio() const override { return 0.002; };
			int range() const override { return 7; }
		};

		std::unique_ptr<Fire> _fire;

		Requirements requirements() const override { return Hands{2}; }
	};
}
