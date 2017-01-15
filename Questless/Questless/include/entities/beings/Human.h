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

#include "entities/beings/CorporealBeing.h"

namespace questless
{
	class Human : public CorporealBeing
	{
	public:
		static constexpr Attributes base_attributes
			{ Vitality{100.0}
			, Spirit{50.0}
			, HealthRegen{0.1}
			, ManaRegen{1.0}
			, Strength{0.0}
			, Endurance{120.0}
			, Stamina{120.0}
			, Agility{0.0}
			, Dexterity{0.0}
			, Stealth{100.0}
			, Vision
				{ Acuity{100.0}
				, IdealLight{100.0}
				, LightTolerance{10.0}
				}
			, Hearing{80.0}
			, Intellect{0.0}
			, Lift{0.0}
			, MinTemp{-100.0}
			, MaxTemp{100.0}
			, Mute{false}
			, Protection::zero()
			, Resistance::zero()
			, Vulnerability::zero()
			, MagicPower
				{ WhiteMagic{100.0}
				, BlackMagic{100.0}
				, GreenMagic{100.0}
				, RedMagic{100.0}
				, BlueMagic{100.0}
				, YellowMagic{100.0}
				}
			, MagicResistance
				{ WhiteMagic{100.0}
				, BlackMagic{100.0}
				, GreenMagic{100.0}
				, RedMagic{100.0}
				, BlueMagic{100.0}
				, YellowMagic{100.0}
				}
			};

		static constexpr double head_vitality = 25.0;
		static constexpr double head_intellect = 100.0;

		static constexpr double torso_vitality = 50.0;
		static constexpr double torso_strength = 20.0;

		static constexpr double arm_vitality = 25.0;
		static constexpr double arm_strength = 15.0;

		static constexpr double hand_vitality = 15.0;
		static constexpr double hand_dexterity = 120.0;

		static constexpr double leg_vitality = 25.0;
		static constexpr double leg_agility = 35.0;
		static constexpr double leg_strength = 15.0;

		static constexpr double foot_vitality = 15.0;
		static constexpr double foot_agility = 15.0;

		Human(Game& game, const std::function<std::unique_ptr<Agent>(Being&)>& make_agent, BeingId id);
		Human(Game& game, std::istream& in);

		void accept(EntityVisitor& visitor) override { return visitor.visit(*this); }
		void accept(EntityVisitor& visitor) const override { return visitor.visit(*this); }

		EntityClass entity_class() const override { return EntityClass::HumanClass; }

		void serialize(std::ostream& out) const override;

		virtual std::string description() const
		{
			return "You already know about humans.";
		}
	protected:
		Body make_body() override;
	};
}

#endif
