//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/corporeal_being.h"

namespace ql
{
	class human : public corporeal_being_base<human>
	{
	public:
		static constexpr ql::stats dflt_base_stats
			{ vitality{100.0}
			, spirit{0.0}
			, health_regen{0.1}
			, mana_regen{1.0}
			, strength{0.0}
			, endurance{120.0}
			, stamina{120.0}
			, agility{0.0}
			, dexterity{0.0}
			, stealth{100.0}
			, vision
				{ acuity{100.0}
				, ideal_illuminance{100.0}
				, darkness_tolerance{25.0}
				, glare_tolerance{100.0}
				}
			, hearing{80.0}
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
		static constexpr intellect head_intellect{100.0};
		static constexpr spirit head_spirit{50.0};
		static constexpr mute head_mute{false};

		static constexpr vitality torso_vitality{50.0};
		static constexpr weight torso_weight{35.0};
		static constexpr strength torso_strength{20.0};

		static constexpr vitality arm_vitality{25.0};
		static constexpr weight arm_weight{10.0};
		static constexpr strength arm_strength{15.0};

		static constexpr vitality hand_vitality{15.0};
		static constexpr weight hand_weight{5.0};
		static constexpr dexterity hand_dexterity{120.0};

		static constexpr vitality leg_vitality{25.0};
		static constexpr weight leg_weight{10.0};
		static constexpr agility leg_agility{35.0};
		static constexpr strength leg_strength{15.0};

		static constexpr vitality foot_vitality{15.0};
		static constexpr weight foot_weight{5.0};
		static constexpr agility foot_agility{15.0};

		human(const std::function<uptr<ql::agent>(being&)>& make_agent, ql::id<being> id = ql::id<being>::make());
		human(std::istream& in);

		ql::entity_class entity_class() const final { return entity_class::human_class; }

		void serialize(std::ostream& out) const final;

		double transparency() const final { return 0.5; }

		std::string const& description() const //! @todo Put this in being or entity?
		{
			return "You already know about humans.";
		}
	protected:
		ql::body make_body() final;
	};
}
