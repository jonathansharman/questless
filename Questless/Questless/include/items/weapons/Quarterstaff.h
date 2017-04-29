//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/weapons/ModalWeapon.h"

//! @todo Implementation file: Quaterstaff.cpp.

namespace questless
{
	class Quarterstaff : public ModalWeaponBase<Quarterstaff>
	{
	public:
		Quarterstaff(Id<Item> id = Id<Item>::make())
			: Item{id}
			, ModalWeaponBase<Quarterstaff>{durability(), std::make_unique<StandardForm>(*this)}
		{}

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
				, _strike{std::make_shared<Strike>(weapon.id)}
				, _jab{std::make_shared<Jab>(weapon.id)}
			{}

			std::string name() const override { return "Quarterstaff"; }

			std::vector<uptr<Action>> actions() override
			{
				std::vector<uptr<Action>> actions;
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
				Damage base_damage() const override { return Bludgeon{24.0}; }
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
				Damage base_damage() const override { return Bludgeon{12.0}; }
				double wind_up() const override { return 0.2; }
				double follow_through() const override { return 0.8; }
				double cooldown() const override { return 1.3; }
				double wear_ratio() const override { return 0.001; }
			};

			std::shared_ptr<Strike> _strike;
			std::shared_ptr<Jab> _jab;
		};

		class HalfStaffForm : public Form
		{
		public:
			HalfStaffForm(ModalWeapon& weapon)
				: Form{weapon}
				, _strike{std::make_shared<Strike>(weapon.id)}
				, _jab{std::make_shared<Jab>(weapon.id)}
			{}

			std::string name() const override { return "Half Staff"; }

			std::vector<uptr<Action>> actions() override
			{
				std::vector<uptr<Action>> actions;
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
				Damage base_damage() const override { return Damage{Bludgeon{18.0}, Protection{Pad{6.0}} + Protection{Deflect{6.0}}}; }
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
				Damage base_damage() const override { return Damage{Bludgeon{9.0}, Protection{Pad{6.0}} + Protection{Deflect{6.0}}}; }
				double wind_up() const override { return 0.16; }
				double follow_through() const override { return 0.64; }
				double cooldown() const override { return 1.04; }
				double wear_ratio() const override { return 0.001; }
			};

			std::shared_ptr<Strike> _strike;
			std::shared_ptr<Jab> _jab;
		};

		Requirements requirements() const override { return Hands{2}; }
	};
}
