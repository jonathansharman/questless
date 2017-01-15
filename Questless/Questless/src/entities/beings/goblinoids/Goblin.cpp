/**
* @file    Goblin.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Goblin class.
*/

#include "entities/beings/goblinoids/Goblin.h"
#include "entities/beings/Body.h"
#include "entities/beings/BodyPart.h"

using namespace units;

namespace questless
{
	Goblin::Goblin(Game& game, const std::function<std::unique_ptr<Agent>(Being&)>& make_agent, BeingId id)
		: CorporealBeing{game, make_agent, id, make_body(), Attributes::of<Goblin>}
	{}

	Goblin::Goblin(Game& game, std::istream& in) : CorporealBeing{game, in, make_body()} {}

	void Goblin::serialize(std::ostream& out) const
	{
		Being::serialize(out);

		out << std::endl;
	}

	Body Goblin::make_body()
	{
		using std::move;

		std::unique_ptr<Hand> left_hand = std::make_unique<Hand>(*this, "Left hand", hand_dexterity, hand_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, std::vector<ScreenRect>{ScreenRect{4, -2, 2, 2}});
		auto left_arm = std::make_unique<Arm>(*this, "Left arm", arm_strength, arm_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, std::vector<ScreenRect>{ScreenRect{4, 6, 1, 1}, ScreenRect{4, 5, 2, 7}});
		left_arm->attach(move(left_hand));

		auto right_hand = std::make_unique<Hand>(*this, "Right hand", hand_dexterity, hand_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, std::vector<ScreenRect>{ScreenRect{-5, -2, 2, 2}});
		auto right_arm = std::make_unique<Arm>(*this, "Right arm", arm_strength, arm_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, std::vector<ScreenRect>{ScreenRect{-4, 6, 1, 1}, ScreenRect{-5, 5, 2, 7}});
		right_arm->attach(move(right_hand));

		auto left_foot = std::make_unique<Foot>(*this, "Left foot", foot_agility, foot_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, std::vector<ScreenRect>{ScreenRect{3, -11, 1, 1}, ScreenRect{1, -10, 2, 2}});
		auto left_leg = std::make_unique<Leg>(*this, "Left leg", leg_agility, leg_strength, leg_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, std::vector<ScreenRect>{ScreenRect{2, -2, 1, 1}, ScreenRect{1, -3, 2, 7}});
		left_leg->attach(move(left_foot));

		auto right_foot = std::make_unique<Foot>(*this, "Right foot", foot_agility, foot_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, std::vector<ScreenRect>{ScreenRect{-3, -11, 1, 1}, ScreenRect{-2, -10, 2, 2}});
		auto right_leg = std::make_unique<Leg>(*this, "Right leg", leg_agility, leg_strength, leg_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, std::vector<ScreenRect>{ScreenRect{-2, -2, 1, 1}, ScreenRect{-2, -3, 2, 7}});
		right_leg->attach(move(right_foot));

		auto torso = std::make_unique<Torso>(*this, "Torso", torso_strength, torso_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), true, std::vector<ScreenRect>{ScreenRect{-3, 6, 7, 4}, ScreenRect{-2, 2, 5, 4}, ScreenRect{-1, -2, 3, 1}});
		torso->attach(move(left_arm));
		torso->attach(move(right_arm));
		torso->attach(move(left_leg));
		torso->attach(move(right_leg));

		auto head = std::make_unique<Head>(*this, "Head", head_intellect, head_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), true, std::vector<ScreenRect>{ScreenRect{-1, 10, 3, 4}});
		head->attach(move(torso));

		return Body{*this, move(head)};
	}
}
