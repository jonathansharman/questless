//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/goblinoids/goblin.h"
#include "entities/beings/body.h"
#include "entities/beings/body_part.h"

using namespace units;

namespace ql
{
	goblin::goblin(const std::function<uptr<ql::agent>(being&)>& make_agent, ql::id<being> id)
		: corporeal_being_base<goblin>{make_agent, id, make_body(), []() { return dflt_base_stats; } }
	{}

	goblin::goblin(std::istream& in) : corporeal_being_base<goblin>{in, make_body()} {}

	void goblin::serialize(std::ostream& out) const
	{
		being::serialize(out);

		out << std::endl;
	}

	body goblin::make_body()
	{
		using std::move;

		uptr<hand> left_hand = std::make_unique<hand>(*this, "Left hand", hand_vitality, hand_weight, hand_dexterity, dmg::protect::zero(), dmg::resist::zero(), dmg::vuln::zero(), std::vector<screen_space::box>{screen_space::box{screen_space::point{4, -2}, screen_space::vector{2, 2}}});
		auto left_arm = std::make_unique<arm>(*this, "Left arm", arm_vitality, arm_weight, arm_strength, dmg::protect::zero(), dmg::resist::zero(), dmg::vuln::zero(), std::vector<screen_space::box>{screen_space::box{screen_space::point{4, 6}, screen_space::vector{1, 1}}, screen_space::box{screen_space::point{4, 5}, screen_space::vector{2, 7}}});
		left_arm->attach(move(left_hand));

		auto right_hand = std::make_unique<hand>(*this, "Right hand", hand_vitality, hand_weight, hand_dexterity, dmg::protect::zero(), dmg::resist::zero(), dmg::vuln::zero(), std::vector<screen_space::box>{screen_space::box{screen_space::point{-5, -2}, screen_space::vector{2, 2}}});
		auto right_arm = std::make_unique<arm>(*this, "Right arm", arm_vitality, arm_weight, arm_strength, dmg::protect::zero(), dmg::resist::zero(), dmg::vuln::zero(), std::vector<screen_space::box>{screen_space::box{screen_space::point{-4, 6}, screen_space::vector{1, 1}}, screen_space::box{screen_space::point{-5, 5}, screen_space::vector{2, 7}}});
		right_arm->attach(move(right_hand));

		auto left_foot = std::make_unique<foot>(*this, "Left foot", foot_vitality, foot_weight, foot_agility, dmg::protect::zero(), dmg::resist::zero(), dmg::vuln::zero(), std::vector<screen_space::box>{screen_space::box{screen_space::point{3, -11}, screen_space::vector{1, 1}}, screen_space::box{screen_space::point{1, -10}, screen_space::vector{2, 2}}});
		auto left_leg = std::make_unique<leg>(*this, "Left leg", leg_vitality, leg_weight, leg_agility, leg_strength, dmg::protect::zero(), dmg::resist::zero(), dmg::vuln::zero(), std::vector<screen_space::box>{screen_space::box{screen_space::point{2, -2}, screen_space::vector{1, 1}}, screen_space::box{screen_space::point{1, -3}, screen_space::vector{2, 7}}});
		left_leg->attach(move(left_foot));

		auto right_foot = std::make_unique<foot>(*this, "Right foot", foot_vitality, foot_weight, foot_agility, dmg::protect::zero(), dmg::resist::zero(), dmg::vuln::zero(), std::vector<screen_space::box>{screen_space::box{screen_space::point{-3, -11}, screen_space::vector{1, 1}}, screen_space::box{screen_space::point{-2, -10}, screen_space::vector{2, 2}}});
		auto right_leg = std::make_unique<leg>(*this, "Right leg", leg_vitality, leg_weight, leg_agility, leg_strength, dmg::protect::zero(), dmg::resist::zero(), dmg::vuln::zero(), std::vector<screen_space::box>{screen_space::box{screen_space::point{-2, -2}, screen_space::vector{1, 1}}, screen_space::box{screen_space::point{-2, -3}, screen_space::vector{2, 7}}});
		right_leg->attach(move(right_foot));

		auto torso = std::make_unique<ql::torso>(*this, "Torso", torso_vitality, torso_weight, torso_strength, dmg::protect::zero(), dmg::resist::zero(), dmg::vuln::zero(), std::vector<screen_space::box>{screen_space::box{screen_space::point{-3, 6}, screen_space::vector{7, 4}}, screen_space::box{screen_space::point{-2, 2}, screen_space::vector{5, 4}}, screen_space::box{screen_space::point{-1, -2}, screen_space::vector{3, 1}}});
		torso->attach(move(left_arm));
		torso->attach(move(right_arm));
		torso->attach(move(left_leg));
		torso->attach(move(right_leg));

		auto head = std::make_unique<ql::head>(*this, "Head", head_vitality, head_weight, head_intellect, head_spirit, dmg::protect::zero(), dmg::resist::zero(), dmg::vuln::zero(), std::vector<screen_space::box>{screen_space::box{screen_space::point{-1, 10}, screen_space::vector{3, 4}}});
		head->attach(move(torso));

		return ql::body{*this, move(head)};
	}
}
