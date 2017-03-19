/**
* @file    Bow.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*/

#pragma once

#include "items/weapons/Weapon.h"

/// @todo Implementation file: Bow.cpp.

namespace questless
{
	class Bow : public Weapon
	{
	public:
		Bow(Id<Item> id = Id<Item>::make()) : Item{id}, Breakable{durability()} {}

		void accept(ItemVisitor& visitor) const override { visitor.visit(*this); }

		std::string name() const override { return "Bow"; }

		double weight() const override { return 3.5; }

		double equip_time() const override { return 15.0; }

		double unequip_time() const override { return 15.0; }

		double durability() const override { return 400.0; }

		virtual Damage base_damage() const override { return Pierce{40.0}; }
		virtual double wind_up() const override { return 5.0; }
		virtual double follow_through() const override { return 0.5; }
		virtual double cooldown() const override { return 1.0; }
		virtual double wear_ratio() const override { return 0.002; };

		virtual std::vector<Action::ptr> actions() override
		{
			std::vector<Action::ptr> actions;
			if (equipped()) {
				actions.push_back(BeginFireArrow::make(*this));
				actions.push_back(Unequip::make(*this));
			} else {
				actions.push_back(Equip::make(*this));
				actions.push_back(Drop::make(*this));
				actions.push_back(Throw::make(*this));
			}
			return actions;
		}
	private:
		static constexpr int range = 7;

		class BeginFireArrow : public Action
		{
		public:
			BeginFireArrow(Bow& bow) : _bow{bow} {}
			static ptr make(Bow& bow) { return std::make_unique<BeginFireArrow>(bow); }
			std::string name() const override { return "Fire"; }
			Action::Complete perform(Being& actor, cont_t cont) override;
		private:
			Bow& _bow;
		};

		class CompleteFireArrow : public Action
		{
		public:
			CompleteFireArrow(Id<Item> bow_id) : _bow_id{bow_id} {}
			static ptr make(Id<Item> bow_id) { return std::make_unique<CompleteFireArrow>(bow_id); }
			std::string name() const override { return ""; }
			Action::Complete perform(Being& actor, cont_t cont) override;
		private:
			Id<Item> _bow_id;
		};

		Requirements requirements() const override { return Hands{2}; }
	};
}
