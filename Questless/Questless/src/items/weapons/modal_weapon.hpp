//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/being.hpp"
#include "items/weapons/weapon.hpp"

namespace ql {
	//! A weapon with different modes of use.
	struct modal_weapon : weapon {
		struct mode {
			mode(modal_weapon& weapon) : _weapon{weapon} {}

			//! The modal weapon of which this is a form.
			modal_weapon& weapon() {
				return _weapon;
			}

			virtual std::string name() const = 0;

			//! The list of actions that can be performed with the weapon.
			virtual std::vector<uptr<action>> actions() = 0;

		private:
			modal_weapon& _weapon;
		};

		virtual ~modal_weapon() = default;

		//! The amount of time required to switch from one form to another.
		virtual tick switch_time() const = 0;

	protected:
		template <typename TargetForm>
		struct switch_mode : action {
			switch_mode(modal_weapon& weapon, std::string name)
				: _weapon{weapon}, _mode{weapon._form}, _name{std::move(name)} {}

			static auto make(modal_weapon& weapon, std::string name) {
				return umake<switch_form<TargetForm>>(weapon, name);
			}

			std::string name() const final {
				return _name;
			}

			complete perform(being& actor, cont cont) final {
				_mode = umake<TargetForm>(_weapon);
				actor.busy_time += _weapon.switch_time();
				return cont(result::success);
			}

		private:
			modal_weapon& _weapon;
			uptr<form>& _mode;
			std::string const _name;
		};

		modal_weapon(ql::integrity integrity, uptr<mode> initial_form)
			: weapon{integrity}, _mode{std::move(initial_form)} {}

		//! This weapon's current mode.
		mode const& current_mode() const {
			return *_mode;
		}

	private:
		uptr<mode> _mode;
	};
}
