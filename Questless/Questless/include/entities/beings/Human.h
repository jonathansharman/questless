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
	class Human : public Being
	{
	public:
		static constexpr double base_vitality = 100.0;
		static constexpr double base_spirit = 50.0;
		static constexpr double base_health_regen = 0.1;
		static constexpr double base_mana_regen = 1.0;
		static constexpr double base_strength = 80.0;
		static constexpr double base_endurance = 120.0;
		static constexpr double base_stamina = 120.0;
		static constexpr double base_agility = 100.0;
		static constexpr double base_dexterity = 120.0;
		static constexpr double base_stealth = 100.0;
		static constexpr double base_visual_acuity = 100.0;
		static constexpr double base_ideal_light = 100.0;
		static constexpr double base_light_tolerance = 10.0;
		static constexpr double base_hearing = 80.0;
		static constexpr double base_intellect = 100.0;
		static constexpr double base_lift = 0.0;
		static constexpr double base_min_temp = -100.0;
		static constexpr double base_max_temp = 100.0;
		static constexpr bool base_mute = false;

		static constexpr double base_white_power = 100.0;
		static constexpr double base_black_power = 100.0;
		static constexpr double base_green_power = 100.0;
		static constexpr double base_red_power = 100.0;
		static constexpr double base_blue_power = 100.0;
		static constexpr double base_yellow_power = 100.0;

		static constexpr double base_white_resistance = 100.0;
		static constexpr double base_black_resistance = 100.0;
		static constexpr double base_green_resistance = 100.0;
		static constexpr double base_red_resistance = 100.0;
		static constexpr double base_blue_resistance = 100.0;
		static constexpr double base_yellow_resistance = 100.0;

		static constexpr double head_vitality = 25.0;
		static constexpr double torso_vitality = 50.0;
		static constexpr double arm_vitality = 25.0;
		static constexpr double hand_vitality = 15.0;
		static constexpr double leg_vitality = 25.0;
		static constexpr double foot_vitality = 15.0;

		Human(std::function<std::unique_ptr<Agent>(Being&)> agent_factory, id_t id);
		Human(std::istream& in);

		void accept(EntityVisitor& visitor) override { return visitor.visit(*this); }
		void accept(EntityVisitor& visitor) const override { return visitor.visit(*this); }

		EntityClass entity_class() const override { return EntityClass::HumanClass; }

		void serialize(std::ostream& out) const override;

		Body make_body() override;

		virtual std::string description() const
		{
			return "You already know about humans.";
		}
	};
}

#endif
