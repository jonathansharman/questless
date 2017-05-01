//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/CorporealBeing.h"

namespace questless
{
	class Human : public CorporealBeingBase<Human>
	{
	public:
		static constexpr Stats dflt_base_stats
			{ Vitality{100.0}
			, Spirit{0.0}
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
				, IdealIlluminance{100.0}
				, DarknessTolerance{25.0}
				, GlareTolerance{100.0}
				}
			, Hearing{80.0}
			, Intellect{0.0}
			, Weight{0.0}
			, MinTemp{-100.0}
			, MaxTemp{100.0}
			, Mute{true}
			, Protection::zero()
			, Resistance::zero()
			, Vulnerability::zero()
			, Magic
				{ WhiteMagic{100.0}
				, BlackMagic{100.0}
				, GreenMagic{100.0}
				, RedMagic{100.0}
				, BlueMagic{100.0}
				, YellowMagic{100.0}
				}
			, Antimagic
				{ WhiteMagic{100.0}
				, BlackMagic{100.0}
				, GreenMagic{100.0}
				, RedMagic{100.0}
				, BlueMagic{100.0}
				, YellowMagic{100.0}
				}
			};

		static constexpr Vitality head_vitality{25.0};
		static constexpr Weight head_weight{5.0};
		static constexpr Intellect head_intellect{100.0};
		static constexpr Spirit head_spirit{50.0};
		static constexpr Mute head_mute{false};

		static constexpr Vitality torso_vitality{50.0};
		static constexpr Weight torso_weight{35.0};
		static constexpr Strength torso_strength{20.0};

		static constexpr Vitality arm_vitality{25.0};
		static constexpr Weight arm_weight{10.0};
		static constexpr Strength arm_strength{15.0};

		static constexpr Vitality hand_vitality{15.0};
		static constexpr Weight hand_weight{5.0};
		static constexpr Dexterity hand_dexterity{120.0};

		static constexpr Vitality leg_vitality{25.0};
		static constexpr Weight leg_weight{10.0};
		static constexpr Agility leg_agility{35.0};
		static constexpr Strength leg_strength{15.0};

		static constexpr Vitality foot_vitality{15.0};
		static constexpr Weight foot_weight{5.0};
		static constexpr Agility foot_agility{15.0};

		Human(const std::function<std::unique_ptr<Agent>(Being&)>& make_agent, Id<Being> id = Id<Being>::make());
		Human(std::istream& in);

		EntityClass entity_class() const override { return EntityClass::HumanClass; }

		void serialize(std::ostream& out) const override;

		std::string const& description() const //! @todo Put this in Being or Entity?
		{
			return "You already know about humans.";
		}
	protected:
		Body make_body() override;
	};
}
