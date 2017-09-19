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

		std::string name() const final { return "Staff (" + form().name() + ')'; }

		double weight() const final { return 10.0; }

		double equip_time() const final { return 1.5; }
		double unequip_time() const final { return 1.0; }

		double durability() const final { return 500.0; }

		double switch_time() const final { return 0.25; }
	private:
		class StandardForm : public Form
		{
		public:
			StandardForm(ModalWeapon& weapon)
				: Form{weapon}
				, _strike{std::make_shared<Strike>(weapon.id)}
				, _jab{std::make_shared<Jab>(weapon.id)}
			{}

			std::string name() const final { return "Quarterstaff"; }

			std::vector<uptr<Action>> actions() final
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
				std::string name() const final { return "Strike"; }
				dmg::Group base_damage() const final { return dmg::Bludgeon{24.0}; }
				double wind_up() const final { return 1.0; }
				double follow_through() const final { return 1.0; }
				double cooldown() const final { return 2.0; }
				double wear_ratio() const final { return 0.001; }
			};
			class Jab : public MeleeAttack
			{
			public:
				using MeleeAttack::MeleeAttack;
				std::string name() const final { return "Jab"; }
				dmg::Group base_damage() const final { return dmg::Bludgeon{12.0}; }
				double wind_up() const final { return 0.2; }
				double follow_through() const final { return 0.8; }
				double cooldown() const final { return 1.3; }
				double wear_ratio() const final { return 0.001; }
			};

			sptr<Strike> _strike;
			sptr<Jab> _jab;
		};

		class HalfStaffForm : public Form
		{
		public:
			HalfStaffForm(ModalWeapon& weapon)
				: Form{weapon}
				, _strike{std::make_shared<Strike>(weapon.id)}
				, _jab{std::make_shared<Jab>(weapon.id)}
			{}

			std::string name() const final { return "Half Staff"; }

			std::vector<uptr<Action>> actions() final
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
				std::string name() const final { return "Strike"; }
				dmg::Group base_damage() const final
				{
					return dmg::Group
						{ dmg::Bludgeon{18.0}
						, dmg::Protect{dmg::Pad{6.0}} + dmg::Protect{dmg::Deflect{6.0}}
						};
				}
				double wind_up() const final { return 0.8; }
				double follow_through() const final { return 0.8; }
				double cooldown() const final { return 1.6; }
				double wear_ratio() const final { return 0.001; }
			};
			class Jab : public MeleeAttack
			{
			public:
				using MeleeAttack::MeleeAttack;
				std::string name() const final { return "Jab"; }
				dmg::Group base_damage() const final { return dmg::Group{dmg::Bludgeon{9.0}, dmg::Protect{dmg::Pad{6.0}} + dmg::Protect{dmg::Deflect{6.0}}}; }
				double wind_up() const final { return 0.16; }
				double follow_through() const final { return 0.64; }
				double cooldown() const final { return 1.04; }
				double wear_ratio() const final { return 0.001; }
			};

			sptr<Strike> _strike;
			sptr<Jab> _jab;
		};

		Requirements requirements() const final { return Hands{2}; }
	};
}
