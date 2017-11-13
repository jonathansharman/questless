//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/corporeal_being.hpp"

namespace ql
{
	class goblin : public corporeal_being_base<goblin>
	{
	public:
		static constexpr ql::stats dflt_base_stats
			{ spirit{0.0}
			, health_regen{0.001}
			, mana_regen{1.0}
			, strength{0.0}
			, endurance{100.0}
			, stamina{100.0}
			, agility{0.0}
			, stealth{300.0}
			, vision
				{ acuity{60.0}
				, ideal_illuminance{100.0}
				, darkness_tolerance{100.0}
				, glare_tolerance{25.0}
				}
			, hearing{180.0}
			, intellect{0.0}
			, weight{0.0}
			, min_temp{-100.0}
			, max_temp{100.0}
			, mute{true}
			, dmg::protect::zero()
			, dmg::resist::zero()
			, dmg::vuln::zero()
			};

		static constexpr vitality head_vitality{25.0};
		static constexpr weight head_weight{5.0};
		static constexpr intellect head_intellect{60.0};
		static constexpr spirit head_spirit{20.0};
		static constexpr mute head_mute{false};

		static constexpr vitality torso_vitality{50.0};
		static constexpr weight torso_weight{35.0};
		static constexpr strength torso_strength{20.0};

		static constexpr vitality arm_vitality{25.0};
		static constexpr weight arm_weight{10.0};
		static constexpr strength arm_strength{20.0};

		static constexpr vitality hand_vitality{15.0};
		static constexpr weight hand_weight{5.0};
		static constexpr dexterity hand_dexterity{100.0};

		static constexpr vitality leg_vitality{25.0};
		static constexpr weight leg_weight{10.0};
		static constexpr agility leg_agility{40.0};
		static constexpr strength leg_strength{20.0};

		static constexpr vitality foot_vitality{15.0};
		static constexpr weight foot_weight{5.0};
		static constexpr agility foot_agility{20.0};

		goblin(const std::function<uptr<ql::agent>(being&)>& make_agent, ql::id<being> id = ql::id<being>::make());
		goblin(std::istream& in);

		ql::entity_class entity_class() const final { return entity_class::goblin_class; }

		void serialize(std::ostream& out) const final;

		double transparency() const final { return 0.5; }

		std::string const& description() const
		{
			return "The goblins are a diminutive and simpleminded humanoid race, known particularly for their bad "
				"tempers and love of mischief. Goblins rarely form large communities, preferring small family groups "
				"or solitude. They are remarkably sneaky, and many an unwary traveller through goblin territory has "
				"lost a coin purse or a limb to a goblin ambush.";
		}
	protected:
		ql::body make_body() final;
	};
}
