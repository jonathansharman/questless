/**
* @file    Quarterstaff.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Quarterstaff class.
*/

#ifndef QUARTERSTAFF_H
#define QUARTERSTAFF_H

#include "items/weapons/ModalWeapon.h"

namespace questless
{
	class Quarterstaff : public ModalWeapon
	{
	public:
		class StandardForm : public Form
		{
		public:
			StandardForm(Quarterstaff& quarterstaff) : Form(quarterstaff) {}

			std::string name() const override { return "Quarterstaff"; }

			Quarterstaff& weapon() { return (Quarterstaff&)Form::weapon(); } /// @todo Ugly cast.

			Damage base_damage() const override { return Damage::from_bludgeon(25.0); }
			double wind_up() const override { return 1.0; }
			double follow_through() const override { return 1.0; }
			double cooldown() const override { return 2.0; }
			double wear_ratio() const override { return 0.001; }

			std::vector<Action::ptr> actions() override
			{
				std::vector<Action::ptr> actions;
				if (weapon().ready()) {
					actions.push_back(MeleeAttack::make(weapon()));
				} else {
					actions.push_back(Ready::make(weapon()));
				}
				actions.push_back(SwitchForm<HalfStaffForm>::make(weapon(), "Switch to half staff"));
				actions.push_back(Drop::make(weapon()));
				actions.push_back(Throw::make(weapon()));
				return actions;
			}
		};

		class HalfStaffForm : public Form
		{
		public:
			HalfStaffForm(Quarterstaff& quarterstaff) : Form(quarterstaff) {}

			std::string name() const override { return "Half Staff"; }

			Quarterstaff& weapon() { return (Quarterstaff&)Form::weapon(); } /// @todo Ugly cast.

			Damage base_damage() const override { return Damage::from_bludgeon(15.0); }
			double wind_up() const override { return 0.75; }
			double follow_through() const override { return 0.75; }
			double cooldown() const override { return 1.5; }
			double wear_ratio() const override { return 0.001; }

			std::vector<Action::ptr> actions() override
			{
				std::vector<Action::ptr> actions;
				if (weapon().ready()) {
					actions.push_back(MeleeAttack::make(weapon()));
				} else {
					actions.push_back(Ready::make(weapon()));
				}
				actions.push_back(Block::make(weapon()));
				actions.push_back(SwitchForm<StandardForm>::make(weapon(), "Switch to quarterstaff"));
				actions.push_back(Drop::make(weapon()));
				actions.push_back(Throw::make(weapon()));
				return actions;
			}
		};

		Quarterstaff() : Breakable{durability()}, _form{std::make_unique<StandardForm>(*this)} {}

		void accept(ItemVisitor& visitor) const override { visitor.visit(*this); }

		std::string name() const override { return "Staff (" + _form->name() + ')'; }

		double weight() const override { return 10.0; }

		double equip_time() const override { return 4.0; }

		double unequip_time() const override { return 4.0; }

		/// @todo Not general enough! Weapons could be equipped on anything.
		int hands() const override { return 2; }

		double durability() const override { return 500; }

		Form& form() const override { return *_form; }

		double switch_time() const override { return 1.0; }
	private:
		template <typename TargetForm>
		class SwitchForm : public Action
		{
		public:
			SwitchForm(Quarterstaff& weapon, std::string name) : _weapon{weapon}, _name{std::move(name)} {}

			static ptr make(Quarterstaff& weapon, std::string name) { return std::make_unique<SwitchForm<TargetForm>>(weapon, name); }

			std::string name() const override { return _name; }

			Action::Complete perform(Being& actor, cont_t cont) override
			{
				_weapon._form = std::make_unique<TargetForm>(_weapon);
				actor.gain_busy_time(_weapon.switch_time());
				return cont(Result::success);
			}
		private:
			Quarterstaff& _weapon;
			const std::string _name;
		};

		template <typename TargetForm>
		friend class SwitchForm; /// @todo Ugly friend.

		std::unique_ptr<Form> _form;
	};
}

#endif
