//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/weapons/Weapon.h"

namespace questless
{
	//! A weapon with different forms or modes of use.
	class ModalWeapon : public Weapon
	{
	public:
		class Form
		{
		public:
			Form(ModalWeapon& weapon) : _weapon{weapon} {}

			//! The modal weapon of which this is a form.
			ModalWeapon& weapon() { return _weapon; }

			virtual std::string name() const = 0;

			//! The list of actions that can be performed with the weapon.
			virtual std::vector<uptr<Action>> actions() = 0;
		private:
			ModalWeapon& _weapon;
		};

		virtual ~ModalWeapon() = default;

		//! The amount of time required to switch from one form to another.
		virtual double switch_time() const = 0;

		std::vector<uptr<Action>> actions() override { return _form->actions(); }
	protected:
		template <typename TargetForm>
		class SwitchForm : public Action
		{
		public:
			SwitchForm(ModalWeapon& weapon, std::string name)
				: _weapon{weapon}, _form{weapon._form}, _name{std::move(name)}
			{}

			static auto make(ModalWeapon& weapon, std::string name)
			{
				return std::make_unique<SwitchForm<TargetForm>>(weapon, name);
			}

			std::string name() const override { return _name; }

			Complete perform(Being& actor, cont_t cont) override
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

		ModalWeapon(double integrity, uptr<Form> initial_form) : Weapon{integrity}, _form{std::move(initial_form)} {}

		//! The weapon's current form.
		Form const& form() const { return *_form; }
	private:
		uptr<Form> _form;
	};

	DEFINE_CONST_ELEMENT_BASE(ModalWeapon, Item)
}
