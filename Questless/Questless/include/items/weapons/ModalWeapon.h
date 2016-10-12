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

			virtual std::string name() const = 0;

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

			/// @return The proportion of dealt damage to be applied as wear.
			virtual double wear_ratio() const = 0;

			/// @return The list of actions that can be performed with the weapon.
			virtual std::vector<Action::ptr> actions() = 0;
		private:
			ModalWeapon& _weapon;
		};

		virtual ~ModalWeapon() = default;

		virtual Form& form() const = 0;

		virtual double switch_time() const = 0;

		Damage base_damage() const override { return form().base_damage(); }
		double wind_up() const override { return form().wind_up(); }
		double follow_through() const override { return form().follow_through(); }
		double cooldown() const override { return form().cooldown(); }
		double wear_ratio() const override { return form().wear_ratio(); }

		std::vector<Action::ptr> actions() override { return form().actions(); }
	protected:
		class SwitchForm : public Action
		{
		public:
			SwitchForm(ModalWeapon& weapon, std::string name = "Switch form") : _weapon{weapon}, _name{std::move(name)} {}

			static ptr make(ModalWeapon& weapon, std::string name = "Switch form") { return std::make_unique<Block>(weapon, name); }

			std::string name() const override { return _name; }

			Action::Complete perform(Being& actor, cont_t cont) override;
		private:
			ModalWeapon& _weapon;
			const std::string _name;
		};
	};
}

#endif
