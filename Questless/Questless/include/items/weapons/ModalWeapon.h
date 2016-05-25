/**
* @file    ModalWeapon.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Weapon class, which represents weapons with more than one mode of use.
*/

#ifndef MODAL_WEAPON_H
#define MODAL_WEAPON_H

#include "items/weapons/Weapon.h"

namespace questless
{
	class ModalWeapon : public Weapon
	{
	public:
		class Form
		{
		public:
			using ptr = std::unique_ptr<Form>;

			Form(ModalWeapon& weapon) : _weapon{weapon} {}

			/// @return The modal weapon of which this is a form.
			ModalWeapon& weapon() { return _weapon; }

			/// @return The weapon's effective damage per hit, accounting for wear.
			Damage damage() const { return base_damage() * (1.0 + _weapon.integrity() / _weapon.durability() / 2.0); }

			/// @return The weapon's base damage per hit.
			virtual Damage base_damage() const = 0;

			/// @return Busy time incurred making the weapon ready.
			virtual double wind_up() const = 0;

			/// @return Busy time incurred after using the weapon.
			virtual double follow_through() const = 0;

			/// @return Time after using the weapon before it can be used again.
			virtual double cooldown() const = 0;

			/// @return The list of actions that can be performed with the weapon.
			virtual std::vector<Action::ptr> actions() = 0;
		private:
			ModalWeapon& _weapon;
		};

		virtual ~ModalWeapon() = default;

		virtual Form& form() const = 0;

		Damage base_damage() const override { return form().base_damage(); }
		double wind_up() const override { return form().wind_up(); }
		double follow_through() const override { return form().follow_through(); }
		double cooldown() const override { return form().cooldown(); }

		std::vector<Action::ptr> actions() override { return form().actions(); }
	};
}

#endif
