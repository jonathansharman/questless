/**
* @file    Human.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Human class.
*/

#ifndef HUMAN_H
#define HUMAN_H

#include "entities/beings/Being.h"

namespace questless
{
	namespace HumanK
	{
		const double vitality = 100.0;
		const double spirit = 50.0;
		const double health_regen = 0.1;
		const double mana_regen = 1.0;
		const double strength = 80.0;
		const double endurance = 120.0;
		const double stamina = 120.0;
		const double agility = 100.0;
		const double dexterity = 120.0;
		const double stealth = 100.0;
		const double vision = 100.0;
		const double light_affinity = 100.0;
		const double hearing = 80.0;
		const double intellect = 100.0;
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

	class Human : public Being
	{
	public:
		Human(std::function<std::unique_ptr<Agent>(Being&)> agent_factory, id_t id);
		Human(std::istream& in);

		void accept(EntityVisitor& visitor) override { return visitor.visit(*this); }

		EntityClass entity_class() const override { return EntityClass::HumanClass; }

		void serialize(std::ostream& out) const override;

		std::unique_ptr<BodyPart> make_body() override;

		virtual std::string description() const
		{
			return "You already know about humans.";
		}
	};
}

#endif
