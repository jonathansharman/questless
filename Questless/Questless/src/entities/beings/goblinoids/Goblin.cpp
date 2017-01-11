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
	Goblin::Goblin(Game& game, const std::function<std::unique_ptr<Agent>(Being&)>& agent_factory, BeingId id)
		: CorporealBeing(game, agent_factory, id, make_body(), Attributes::of<Goblin>())
	{}

	Goblin::Goblin(Game& game, std::istream& in) : CorporealBeing(game, in, make_body()) {}

	void Goblin::serialize(std::ostream& out) const
	{
		Being::serialize(out);

		out << std::endl;
	}

	Body Goblin::make_body()
	{
		using std::move;

		auto left_hand = BodyPart::make<Hand>(*this, "Left hand", {}, hand_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, {ScreenRect{4, -2, 2, 2}});
		auto left_arm = BodyPart::make<Arm>(*this, "Left arm", {}, arm_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, {ScreenRect{4, 6, 1, 1}, ScreenRect{4, 5, 2, 7}});
		left_arm->attach(move(left_hand));

		auto right_hand = BodyPart::make<Hand>(*this, "Right hand", {}, hand_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, {ScreenRect{-5, -2, 2, 2}});
		auto right_arm = BodyPart::make<Arm>(*this, "Right arm", {}, arm_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, {ScreenRect{-4, 6, 1, 1}, ScreenRect{-5, 5, 2, 7}});
		right_arm->attach(move(right_hand));

		// 120 total agility.

		auto left_foot = BodyPart::make<Foot>(*this, "Left foot", Modifier::make_vector(AgilityModifier::make(20.0)), foot_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, {ScreenRect{3, -11, 1, 1}, ScreenRect{1, -10, 2, 2}});
		auto left_leg = BodyPart::make<Leg>(*this, "Left leg", Modifier::make_vector(AgilityModifier::make(40.0)), leg_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, {ScreenRect{2, -2, 1, 1}, ScreenRect{1, -3, 2, 7}});
		left_leg->attach(move(left_foot));

		auto right_foot = BodyPart::make<Foot>(*this, "Right foot", Modifier::make_vector(AgilityModifier::make(20.0)), foot_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, {ScreenRect{-3, -11, 1, 1}, ScreenRect{-2, -10, 2, 2}});
		auto right_leg = BodyPart::make<Leg>(*this, "Right leg", Modifier::make_vector(AgilityModifier::make(40.0)), leg_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, {ScreenRect{-2, -2, 1, 1}, ScreenRect{-2, -3, 2, 7}});
		right_leg->attach(move(right_foot));

		auto torso = BodyPart::make<Torso>(*this, "Torso", {}, torso_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), true, {ScreenRect{-3, 6, 7, 4}, ScreenRect{-2, 2, 5, 4}, ScreenRect{-1, -2, 3, 1}});
		torso->attach(move(left_arm));
		torso->attach(move(right_arm));
		torso->attach(move(left_leg));
		torso->attach(move(right_leg));

		auto head = BodyPart::make<Head>(*this, "Head", {}, head_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), true, {ScreenRect{-1, 10, 3, 4}});
		head->attach(move(torso));

		return Body{*this, move(head)};
	}
}
