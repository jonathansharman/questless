//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "items/weapons/weapon.hpp"

namespace ql
{
	//! A weapon with different forms or modes of use.
	class modal_weapon : public weapon
	{
	public:
		class form
		{
		public:
			form(modal_weapon& weapon) : _weapon{weapon} {}

			//! The modal weapon of which this is a form.
			modal_weapon& weapon() { return _weapon; }

			virtual std::string name() const = 0;

			//! The list of actions that can be performed with the weapon.
			virtual std::vector<uptr<action>> actions() = 0;
		private:
			modal_weapon& _weapon;
		};

		virtual ~modal_weapon() = default;

		//! The amount of time required to switch from one form to another.
		virtual double switch_time() const = 0;

		std::vector<uptr<action>> actions() final { return _form->actions(); }
	protected:
		template <typename TargetForm>
		class switch_form : public action
		{
		public:
			switch_form(modal_weapon& weapon, std::string name)
				: _weapon{weapon}, _form{weapon._form}, _name{std::move(name)}
			{}

			static auto make(modal_weapon& weapon, std::string name)
			{
				return std::make_unique<switch_form<TargetForm>>(weapon, name);
			}

			std::string name() const final { return _name; }

			complete perform(being& actor, cont cont) final
			{
				_form = std::make_unique<TargetForm>(_weapon);
				actor.busy_time += _weapon.switch_time();
				return cont(result::success);
			}
		private:
			modal_weapon& _weapon;
			uptr<form>& _form;
			std::string const _name;
		};

		modal_weapon(double integrity, uptr<form> initial_form) : weapon{integrity}, _form{std::move(initial_form)} {}

		//! This weapon's current form.
		form const& current_form() const { return *_form; }
	private:
		uptr<form> _form;
	};

	DEFINE_ELEMENT_BASE(modal_weapon, item)
}
