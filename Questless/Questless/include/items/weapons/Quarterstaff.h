/**
* @file    Quarterstaff.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*/

#pragma once

#include "items/weapons/ModalWeapon.h"

/// @todo Implementation file: Quaterstaff.cpp.

namespace questless
{
	class Quarterstaff : public ModalWeapon
	{
	public:
		class StandardForm : public Form
		{
		public:
			StandardForm(ModalWeapon& weapon) : Form{weapon} {}

			std::string name() const override { return "Quarterstaff"; }

			Damage base_damage() const override { return Bludgeon{25.0}; }
			double wind_up() const override { return 1.0; }
			double follow_through() const override { return 1.0; }
			double cooldown() const override { return 2.0; }
			double wear_ratio() const override { return 0.001; }

			std::vector<Action::ptr> actions() override
			{
				std::vector<Action::ptr> actions;
				if (weapon().equipped()) {
					actions.push_back(BeginMeleeAttack::make(weapon()));
					actions.push_back(SwitchForm<HalfStaffForm>::make(weapon(), "Switch to half-staff grip"));
					actions.push_back(Unequip::make(weapon()));
				} else {
					actions.push_back(Equip::make(weapon()));
					actions.push_back(Drop::make(weapon()));
					actions.push_back(Throw::make(weapon()));
				}
				return actions;
			}
		};

		class HalfStaffForm : public Form
		{
		public:
			HalfStaffForm(ModalWeapon& weapon) : Form{weapon} {}

			std::string name() const override { return "Half Staff"; }

			Damage base_damage() const override { return Bludgeon{15.0}; }
			double wind_up() const override { return 0.75; }
			double follow_through() const override { return 0.75; }
			double cooldown() const override { return 1.5; }
			double wear_ratio() const override { return 0.001; }

			std::vector<Action::ptr> actions() override
			{
				std::vector<Action::ptr> actions;
				if (weapon().equipped()) {
					actions.push_back(BeginMeleeAttack::make(weapon()));
					actions.push_back(Block::make(weapon()));
					actions.push_back(SwitchForm<StandardForm>::make(weapon(), "Switch to quarterstaff grip"));
					actions.push_back(Unequip::make(weapon()));
				} else {
					actions.push_back(Equip::make(weapon()));
					actions.push_back(Drop::make(weapon()));
					actions.push_back(Throw::make(weapon()));
				}
				return actions;
			}
		};

		Quarterstaff(Id<Item> id = Id<Item>::make()) : Item{id}, Breakable{durability()}, ModalWeapon{std::make_unique<StandardForm>(*this)} {}

		void accept(ItemVisitor& visitor) const override { visitor.visit(*this); }

		std::string name() const override { return "Staff (" + form().name() + ')'; }

		double weight() const override { return 10.0; }

		double equip_time() const override { return 1.5; }
		double unequip_time() const override { return 1.0; }

		double durability() const override { return 500.0; }

		double switch_time() const override { return 1.0; }
	private:
		Requirements requirements() const override { return Hands{2}; }
	};
}
