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

#include "entities/beings/CorporealBeing.h"

namespace questless
{
	class Goblin : public CorporealBeing
	{
	public:
		static constexpr Attributes base_attributes
			{ Vitality{100.0}
			, Spirit{20.0}
			, HealthRegen{0.1}
			, ManaRegen{1.0}
			, Strength{0.0}
			, Endurance{100.0}
			, Stamina{100.0}
			, Agility{0.0}
			, Dexterity{0.0}
			, Stealth{300.0}
			, Vision
				{ Acuity{60.0}
				, IdealLight{100.0}
				, LightTolerance{10.0}
				}
			, Hearing{180.0}
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
		static constexpr double head_intellect = 60.0;

		static constexpr double torso_vitality = 50.0;
		static constexpr double torso_strength = 20.0;

		static constexpr double arm_vitality = 25.0;
		static constexpr double arm_strength = 20.0;

		static constexpr double hand_vitality = 15.0;
		static constexpr double hand_dexterity = 100.0;

		static constexpr double leg_vitality = 25.0;
		static constexpr double leg_agility = 40.0;
		static constexpr double leg_strength = 20.0;

		static constexpr double foot_vitality = 15.0;
		static constexpr double foot_agility = 20.0;

		Goblin(Game& game, const std::function<std::unique_ptr<Agent>(Being&)>& make_agent, BeingId id);
		Goblin(Game& game, std::istream& in);

		void accept(EntityVisitor& visitor) override { return visitor.visit(*this); }
		void accept(EntityVisitor& visitor) const override { return visitor.visit(*this); }

		EntityClass entity_class() const override { return EntityClass::GoblinClass; }

		void serialize(std::ostream& out) const override;

		const std::string& description() const { return "The goblins are a diminutive and simpleminded humanoid race, known "
			"particularly for their bad tempers and love of mischief. Goblins rarely form large communities, preferring small "
			"family groups or solitude. They are remarkably sneaky, and many an unwary traveller through goblin territory has "
			"lost a coin purse or a limb to a goblin ambush."; }
	protected:
		Body make_body() override;
	};
}

#endif
