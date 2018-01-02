//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/weapons/modal_weapon.hpp"

//! @todo Implementation file: Quaterstaff.cpp.

namespace ql
{
	class quarterstaff : public modal_weapon_base<quarterstaff>
	{
	public:
		quarterstaff(ql::id<item> id = ql::id<item>::make())
			: item{id}
			, modal_weapon_base<quarterstaff>{durability(), umake<standard_form>(*this)}
		{}

		std::string name() const final { return "Staff (" + current_form().name() + ')'; }

		double weight() const final { return 10.0; }

		double equip_time() const final { return 1.5; }
		double unequip_time() const final { return 1.0; }

		double durability() const final { return 500.0; }

		double switch_time() const final { return 0.25; }
	private:
		class standard_form : public form
		{
		public:
			standard_form(modal_weapon& weapon)
				: form{weapon}
				, _strike{smake<strike>(weapon.id)}
				, _jab{smake<jab>(weapon.id)}
			{}

			std::string name() const final { return "Quarterstaff"; }

			std::vector<uptr<action>> actions() final
			{
				std::vector<uptr<action>> actions;
				if (weapon().equipped()) {
					actions.push_back(_strike->launch());
					actions.push_back(_jab->launch());
					actions.push_back(switch_form<half_staff_form>::make(weapon(), "Switch to half-staff grip"));
					actions.push_back(unequip::make(weapon()));
				} else {
					actions.push_back(equip::make(weapon()));
					actions.push_back(drop::make(weapon()));
					actions.push_back(toss::make(weapon()));
				}
				return actions;
			}
		private:
			class strike : public melee_attack
			{
			public:
				using melee_attack::melee_attack;
				std::string name() const final { return "Strike"; }
				dmg::group base_damage() const final { return dmg::bludgeon{24.0}; }
				double wind_up() const final { return 1.0; }
				double follow_through() const final { return 1.0; }
				double cooldown() const final { return 2.0; }
				double wear_ratio() const final { return 0.001; }
			};
			class jab : public melee_attack
			{
			public:
				using melee_attack::melee_attack;
				std::string name() const final { return "Jab"; }
				dmg::group base_damage() const final { return dmg::bludgeon{12.0}; }
				double wind_up() const final { return 0.2; }
				double follow_through() const final { return 0.8; }
				double cooldown() const final { return 1.3; }
				double wear_ratio() const final { return 0.001; }
			};

			sptr<strike> _strike;
			sptr<jab> _jab;
		};

		class half_staff_form : public form
		{
		public:
			half_staff_form(modal_weapon& weapon)
				: form{weapon}
				, _strike{smake<strike>(weapon.id)}
				, _jab{smake<jab>(weapon.id)}
			{}

			std::string name() const final { return "Half Staff"; }

			std::vector<uptr<action>> actions() final
			{
				std::vector<uptr<action>> actions;
				if (weapon().equipped()) {
					actions.push_back(_strike->launch());
					actions.push_back(_jab->launch());
					actions.push_back(switch_form<standard_form>::make(weapon(), "Switch to quarterstaff grip"));
					actions.push_back(unequip::make(weapon()));
				} else {
					actions.push_back(equip::make(weapon()));
					actions.push_back(drop::make(weapon()));
					actions.push_back(toss::make(weapon()));
				}
				return actions;
			}
		private:
			class strike : public melee_attack
			{
			public:
				using melee_attack::melee_attack;
				std::string name() const final { return "Strike"; }
				dmg::group base_damage() const final
				{
					return dmg::group
						{ dmg::bludgeon{18.0}
						, dmg::protect{dmg::pad{6.0}} + dmg::protect{dmg::deflect{6.0}}
						};
				}
				double wind_up() const final { return 0.8; }
				double follow_through() const final { return 0.8; }
				double cooldown() const final { return 1.6; }
				double wear_ratio() const final { return 0.001; }
			};
			class jab : public melee_attack
			{
			public:
				using melee_attack::melee_attack;
				std::string name() const final { return "Jab"; }
				dmg::group base_damage() const final
				{
					return dmg::group
						{ dmg::bludgeon{9.0}
						, dmg::protect{dmg::pad{6.0}} + dmg::protect{dmg::deflect{6.0}}
						};
				}
				double wind_up() const final { return 0.16; }
				double follow_through() const final { return 0.64; }
				double cooldown() const final { return 1.04; }
				double wear_ratio() const final { return 0.001; }
			};

			sptr<strike> _strike;
			sptr<jab> _jab;
		};

		body_part_counts requirements() const final { return hands{2}; }
	};
}
