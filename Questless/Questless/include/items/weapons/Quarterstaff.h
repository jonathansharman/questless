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

			Damage base_damage() const override { return Damage::from_bludgeon(10.0); }
			double wind_up() const override { return 1.0; }
			double follow_through() const override { return 1.0; }
			double cooldown() const override { return 2.0; }

			std::vector<Action::ptr> actions() override
			{
				std::vector<Action::ptr> actions;
				if (weapon().ready()) {
					actions.push_back(Attack::make(weapon()));
				} else {
					actions.push_back(Ready::make(weapon()));
				}
				actions.push_back(Drop::make(weapon()));
				actions.push_back(Throw::make(weapon()));
				return actions;
			}
		};

		class HalfStaffForm : public Form
		{
		public:
			HalfStaffForm(Quarterstaff& quarterstaff) : Form(quarterstaff) {}

			Damage base_damage() const override { return Damage::from_bludgeon(5.0); }
			double wind_up() const override { return 0.75; }
			double follow_through() const override { return 0.75; }
			double cooldown() const override { return 1.5; }

			std::vector<Action::ptr> actions() override
			{
				std::vector<Action::ptr> actions;
				if (weapon().ready()) {
					actions.push_back(Attack::make(weapon()));
				} else {
					actions.push_back(Ready::make(weapon()));
				}
				actions.push_back(Block::make(weapon()));
				actions.push_back(Drop::make(weapon()));
				actions.push_back(Throw::make(weapon()));
				return actions;
			}
		};

		Quarterstaff() : Breakable(durability()), _form{std::make_unique<StandardForm>(*this)} {}

		void accept(ItemVisitor& visitor) const override { visitor.visit(*this); }

		std::string name() const override { return "Quarterstaff"; }

		double weight() const override { return 10.0; }

		double equip_time() const override { return 4.0; }

		double unequip_time() const override { return 4.0; }

		/// @todo Not general enough! Weapons could be equipped on anything.
		int hands() const override { return 2; }

		double durability() const override { return 500; }

		Form& form() const override { return *_form; }
	private:
		std::unique_ptr<Form> _form;
	};
}

#endif
