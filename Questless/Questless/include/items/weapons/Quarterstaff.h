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
		Quarterstaff(Id<Item> id = Id<Item>::make())
			: Item{id}
			, ModalWeapon{durability(), std::make_unique<StandardForm>(*this)}
		{}

		void accept(ItemVisitor& visitor) const override { visitor.visit(*this); }

		std::string name() const override { return "Staff (" + form().name() + ')'; }

		double weight() const override { return 10.0; }

		double equip_time() const override { return 1.5; }
		double unequip_time() const override { return 1.0; }

		double durability() const override { return 500.0; }

		double switch_time() const override { return 0.25; }
	private:
		class StandardForm : public Form
		{
		public:
			StandardForm(ModalWeapon& weapon)
				: Form{weapon}
				, _strike{std::make_unique<Strike>(weapon)}
				, _jab{std::make_unique<Jab>(weapon)}
			{}

			std::string name() const override { return "Quarterstaff"; }

			std::vector<Action::ptr> actions() override
			{
				std::vector<Action::ptr> actions;
				if (weapon().equipped()) {
					actions.push_back(_strike->launch());
					actions.push_back(_jab->launch());
					actions.push_back(SwitchForm<HalfStaffForm>::make(weapon(), "Switch to half-staff grip"));
					actions.push_back(Unequip::make(weapon()));
				} else {
					actions.push_back(Equip::make(weapon()));
					actions.push_back(Drop::make(weapon()));
					actions.push_back(Throw::make(weapon()));
				}
				return actions;
			}
		private:
			class Strike : public MeleeAttack
			{
			public:
				using MeleeAttack::MeleeAttack;
				std::string name() const override { return "Strike"; }
				Damage base_damage() const override { return Bludgeon{25.0}; }
				double wind_up() const override { return 1.0; }
				double follow_through() const override { return 1.0; }
				double cooldown() const override { return 2.0; }
				double wear_ratio() const override { return 0.001; }
			};
			class Jab : public MeleeAttack
			{
			public:
				using MeleeAttack::MeleeAttack;
				std::string name() const override { return "Jab"; }
				Damage base_damage() const override { return Bludgeon{12.5}; }
				double wind_up() const override { return 0.25; }
				double follow_through() const override { return 0.625; }
				double cooldown() const override { return 1.25; }
				double wear_ratio() const override { return 0.001; }
			};

			std::unique_ptr<Strike> _strike;
			std::unique_ptr<Jab> _jab;
		};

		class HalfStaffForm : public Form
		{
		public:
			HalfStaffForm(ModalWeapon& weapon)
				: Form{weapon}
				, _strike{std::make_unique<Strike>(weapon)}
				, _jab{std::make_unique<Jab>(weapon)}
			{}

			std::string name() const override { return "Half Staff"; }

			std::vector<Action::ptr> actions() override
			{
				std::vector<Action::ptr> actions;
				if (weapon().equipped()) {
					actions.push_back(_strike->launch());
					actions.push_back(_jab->launch());
					actions.push_back(SwitchForm<StandardForm>::make(weapon(), "Switch to quarterstaff grip"));
					actions.push_back(Unequip::make(weapon()));
				} else {
					actions.push_back(Equip::make(weapon()));
					actions.push_back(Drop::make(weapon()));
					actions.push_back(Throw::make(weapon()));
				}
				return actions;
			}
		private:
			class Strike : public MeleeAttack
			{
			public:
				using MeleeAttack::MeleeAttack;
				std::string name() const override { return "Strike"; }
				Damage base_damage() const override { return Bludgeon{18.0}; }
				double wind_up() const override { return 0.8; }
				double follow_through() const override { return 0.8; }
				double cooldown() const override { return 1.6; }
				double wear_ratio() const override { return 0.001; }
			};
			class Jab : public MeleeAttack
			{
			public:
				using MeleeAttack::MeleeAttack;
				std::string name() const override { return "Jab"; }
				Damage base_damage() const override { return Bludgeon{7.5}; }
				double wind_up() const override { return 0.2; }
				double follow_through() const override { return 0.5; }
				double cooldown() const override { return 1.0; }
				double wear_ratio() const override { return 0.001; }
			};

			std::unique_ptr<Strike> _strike;
			std::unique_ptr<Jab> _jab;
		};

		Requirements requirements() const override { return Hands{2}; }
	};
}
