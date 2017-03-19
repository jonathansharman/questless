/**
* @file    ModalWeapon.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Weapon class, which represents weapons with more than one mode of use.
*/

#pragma once

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
			////
			Damage damage() const { return base_damage() * (1.0 + _weapon.integrity / _weapon.durability() / 2.0); }

			/// @return The weapon's base damage per hit.
			////
			virtual Damage base_damage() const = 0;

			/// @return Busy time incurred making the weapon ready.
			////
			virtual double wind_up() const = 0;

			/// @return Busy time incurred after using the weapon.
			////
			virtual double follow_through() const = 0;

			/// @return Time after using the weapon before it can be used again.
			////
			virtual double cooldown() const = 0;

			/// @return The proportion of dealt damage to be applied as wear.
			////
			virtual double wear_ratio() const = 0;

			/// @return The list of actions that can be performed with the weapon.
			////
			virtual std::vector<Action::ptr> actions() = 0;
		private:
			ModalWeapon& _weapon;
		};

		virtual ~ModalWeapon() = default;

		virtual double switch_time() const = 0;

		Damage base_damage() const override { return _form->base_damage(); }
		double wind_up() const override { return _form->wind_up(); }
		double follow_through() const override { return _form->follow_through(); }
		double cooldown() const override { return _form->cooldown(); }
		double wear_ratio() const override { return _form->wear_ratio(); }

		std::vector<Action::ptr> actions() override { return _form->actions(); }
	protected:
		template <typename TargetForm>
		class SwitchForm : public Action
		{
		public:
			SwitchForm(ModalWeapon& weapon, std::string name)
				: _weapon{weapon}, _form{weapon._form}, _name{std::move(name)}
			{}

			static ptr make(ModalWeapon& weapon, std::string name)
			{
				return std::make_unique<SwitchForm<TargetForm>>(weapon, name);
			}

			std::string name() const override { return _name; }

			Action::Complete perform(Being& actor, cont_t cont) override
			{
				_form = std::make_unique<TargetForm>(_weapon);
				actor.busy_time += _weapon.switch_time();
				return cont(Result::success);
			}
		private:
			ModalWeapon& _weapon;
			std::unique_ptr<Form>& _form;
			std::string const _name;
		};

		ModalWeapon(Form::ptr initial_form) : _form{std::move(initial_form)} {}

		/// @return The weapon's current form.
		////
		Form const& form() const { return *_form; }
	private:
		Form::ptr _form;
	};
}
