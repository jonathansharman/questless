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

			/// @return The list of actions that can be performed with the weapon.
			////
			virtual std::vector<Action::ptr> actions() = 0;
		private:
			ModalWeapon& _weapon;
		};

		virtual ~ModalWeapon() = default;

		virtual double switch_time() const = 0;

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

		ModalWeapon(double integrity, Form::ptr initial_form) : Weapon{integrity}, _form{std::move(initial_form)} {}

		/// @return The weapon's current form.
		////
		Form const& form() const { return *_form; }
	private:
		Form::ptr _form;
	};
}
