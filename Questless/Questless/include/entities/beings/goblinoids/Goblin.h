/**
* @file    Goblin.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Goblin class.
*/

#ifndef GOBLIN_H
#define GOBLIN_H

#include "entities/beings/Being.h"

namespace questless
{
	namespace GoblinK
	{
		const double vitality = 100.0;
		const double spirit = 20.0;
		const double health_regen = 0.1;
		const double mana_regen = 1.0;
		const double strength = 100.0;
		const double endurance = 100.0;
		const double stamina = 100.0;
		const double agility = 120.0;
		const double dexterity = 100.0;
		const double stealth = 300.0;
		const double vision = 60.0;
		const double light_affinity = 100.0;
		const double hearing = 180.0;
		const double intellect = 60.0;
		const double lift = 0.0;
		const double min_temp = 100.0;
		const double max_temp = -100.0;
		const unsigned hands = 2;
		const bool mute = false;

		const double white_power = 100.0;
		const double black_power = 100.0;
		const double green_power = 100.0;
		const double red_power = 100.0;
		const double blue_power = 100.0;
		const double yellow_power = 100.0;

		const double white_resistance = 100.0;
		const double black_resistance = 100.0;
		const double green_resistance = 100.0;
		const double red_resistance = 100.0;
		const double blue_resistance = 100.0;
		const double yellow_resistance = 100.0;
	}

	class Goblin : public Being
	{
	public:
		Goblin(std::function<std::unique_ptr<Agent>(Being&)> agent_factory, id_t id);
		Goblin(std::istream& in);

		void accept(EntityVisitor& visitor) override { return visitor.visit(*this); }

		EntityClass entity_class() const override { return EntityClass::GoblinClass; }

		void serialize(std::ostream& out) const override;

		std::unique_ptr<BodyPart> make_body() override;

		const string& description() const { return "The goblins are a diminutive and simpleminded humanoid race, known "
			"particularly for their bad tempers and love of mischief. Goblins rarely form large communities, preferring small "
			"family groups or solitude. They are remarkably sneaky, and many an unwary traveller through goblin territory has "
			"lost a coin purse or a limb to a goblin ambush."; }
	};
}

#endif