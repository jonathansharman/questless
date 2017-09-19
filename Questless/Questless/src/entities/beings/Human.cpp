//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/Human.h"
#include "entities/beings/Body.h"
#include "entities/beings/BodyPart.h"

using namespace units;

namespace questless
{
	Human::Human(const std::function<uptr<Agent>(Being&)>& make_agent, Id<Being> id)
		: CorporealBeingBase<Human>{make_agent, id, make_body(), []() { return dflt_base_stats; }}
	{}

	Human::Human(std::istream& in) : CorporealBeingBase<Human>{in, make_body()} {}

	void Human::serialize(std::ostream& out) const
	{
		Being::serialize(out);
		
		out << std::endl;
	}

	Body Human::make_body()
	{
		using std::move;

		uptr<Hand> left_hand = std::make_unique<Hand>(*this, "Left hand", hand_vitality, hand_weight, hand_dexterity, dmg::Protect::zero(), dmg::Resist::zero(), dmg::Vuln::zero(), std::vector<ScreenSpace::Box>{ScreenSpace::Box{ScreenSpace::Point{4, -2}, ScreenSpace::Vector{2, 2}}});
		auto left_arm = std::make_unique<Arm>(*this, "Left arm", arm_vitality, arm_weight, arm_strength, dmg::Protect::zero(), dmg::Resist::zero(), dmg::Vuln::zero(), std::vector<ScreenSpace::Box>{ScreenSpace::Box{ScreenSpace::Point{4, 6}, ScreenSpace::Vector{1, 1}}, ScreenSpace::Box{ScreenSpace::Point{4, 5}, ScreenSpace::Vector{2, 7}}});
		left_arm->attach(move(left_hand));

		auto right_hand = std::make_unique<Hand>(*this, "Right hand", hand_vitality, hand_weight, hand_dexterity, dmg::Protect::zero(), dmg::Resist::zero(), dmg::Vuln::zero(), std::vector<ScreenSpace::Box>{ScreenSpace::Box{ScreenSpace::Point{-5, -2}, ScreenSpace::Vector{2, 2}}});
		auto right_arm = std::make_unique<Arm>(*this, "Right arm", arm_vitality, arm_weight, arm_strength, dmg::Protect::zero(), dmg::Resist::zero(), dmg::Vuln::zero(), std::vector<ScreenSpace::Box>{ScreenSpace::Box{ScreenSpace::Point{-4, 6}, ScreenSpace::Vector{1, 1}}, ScreenSpace::Box{ScreenSpace::Point{-5, 5}, ScreenSpace::Vector{2, 7}}});
		right_arm->attach(move(right_hand));

		auto left_foot = std::make_unique<Foot>(*this, "Left foot", foot_vitality, foot_weight, foot_agility, dmg::Protect::zero(), dmg::Resist::zero(), dmg::Vuln::zero(), std::vector<ScreenSpace::Box>{ScreenSpace::Box{ScreenSpace::Point{3, -11}, ScreenSpace::Vector{1, 1}}, ScreenSpace::Box{ScreenSpace::Point{1, -10}, ScreenSpace::Vector{2, 2}}});
		auto left_leg = std::make_unique<Leg>(*this, "Left leg", leg_vitality, leg_weight, leg_agility, leg_strength, dmg::Protect::zero(), dmg::Resist::zero(), dmg::Vuln::zero(), std::vector<ScreenSpace::Box>{ScreenSpace::Box{ScreenSpace::Point{2, -2}, ScreenSpace::Vector{1, 1}}, ScreenSpace::Box{ScreenSpace::Point{1, -3}, ScreenSpace::Vector{2, 7}}});
		left_leg->attach(move(left_foot));

		auto right_foot = std::make_unique<Foot>(*this, "Right foot", foot_vitality, foot_weight, foot_agility, dmg::Protect::zero(), dmg::Resist::zero(), dmg::Vuln::zero(), std::vector<ScreenSpace::Box>{ScreenSpace::Box{ScreenSpace::Point{-3, -11}, ScreenSpace::Vector{1, 1}}, ScreenSpace::Box{ScreenSpace::Point{-2, -10}, ScreenSpace::Vector{2, 2}}});
		auto right_leg = std::make_unique<Leg>(*this, "Right leg", leg_vitality, leg_weight, leg_agility, leg_strength, dmg::Protect::zero(), dmg::Resist::zero(), dmg::Vuln::zero(), std::vector<ScreenSpace::Box>{ScreenSpace::Box{ScreenSpace::Point{-2, -2}, ScreenSpace::Vector{1, 1}}, ScreenSpace::Box{ScreenSpace::Point{-2, -3}, ScreenSpace::Vector{2, 7}}});
		right_leg->attach(move(right_foot));

		auto torso = std::make_unique<Torso>(*this, "Torso", torso_vitality, torso_weight, torso_strength, dmg::Protect::zero(), dmg::Resist::zero(), dmg::Vuln::zero(), std::vector<ScreenSpace::Box>{ScreenSpace::Box{ScreenSpace::Point{-3, 6}, ScreenSpace::Vector{7, 4}}, ScreenSpace::Box{ScreenSpace::Point{-2, 2}, ScreenSpace::Vector{5, 4}}, ScreenSpace::Box{ScreenSpace::Point{-1, -2}, ScreenSpace::Vector{3, 1}}});
		torso->attach(move(left_arm));
		torso->attach(move(right_arm));
		torso->attach(move(left_leg));
		torso->attach(move(right_leg));

		auto head = std::make_unique<Head>(*this, "Head", head_vitality, head_weight, head_intellect, head_spirit, dmg::Protect::zero(), dmg::Resist::zero(), dmg::Vuln::zero(), std::vector<ScreenSpace::Box>{ScreenSpace::Box{ScreenSpace::Point{-1, 10}, ScreenSpace::Vector{3, 4}}});
		head->attach(move(torso));

		return Body{*this, move(head)};
	}
}
