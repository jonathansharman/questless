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
#include "agents/Agent.h"

using namespace units;

namespace questless
{
	Goblin::Goblin(Game& game, std::function<std::unique_ptr<Agent>(Being&)> agent_factory, BeingId id)
		: CorporealBeing(game, std::move(agent_factory), id, make_body(), Attributes::of<Goblin>())
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

		auto left_hand = BodyPart::make<Hand>(*this, "Left hand", Attributes::modifiers_t{}, hand_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, {ScreenRect{4, -2, 2, 2}});
		auto left_arm = BodyPart::make<Arm>(*this, "Left arm", Attributes::modifiers_t{}, arm_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, {ScreenRect{4, 6, 1, 1}, ScreenRect{4, 5, 2, 7}});
		left_arm->attach(move(left_hand));

		auto right_hand = BodyPart::make<Hand>(*this, "Right hand", Attributes::modifiers_t{}, hand_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, {ScreenRect{-5, -2, 2, 2}});
		auto right_arm = BodyPart::make<Arm>(*this, "Right arm", Attributes::modifiers_t{}, arm_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, {ScreenRect{-4, 6, 1, 1}, ScreenRect{-5, 5, 2, 7}});
		right_arm->attach(move(right_hand));

		// 120 total agility.

		Attributes::modifiers_t left_foot_modifiers;
		left_foot_modifiers.push_back(std::make_unique<Attributes::AgilityModifier>(20.0));
		auto left_foot = BodyPart::make<Foot>(*this, "Left foot", move(left_foot_modifiers), foot_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, {ScreenRect{3, -11, 1, 1}, ScreenRect{1, -10, 2, 2}});
		Attributes::modifiers_t left_leg_modifiers;
		left_leg_modifiers.push_back(std::make_unique<Attributes::AgilityModifier>(40.0));
		auto left_leg = BodyPart::make<Leg>(*this, "Left leg", move(left_leg_modifiers), leg_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, {ScreenRect{2, -2, 1, 1}, ScreenRect{1, -3, 2, 7}});
		left_leg->attach(move(left_foot));

		Attributes::modifiers_t right_foot_modifiers;
		right_foot_modifiers.push_back(std::make_unique<Attributes::AgilityModifier>(20.0));
		auto right_foot = BodyPart::make<Foot>(*this, "Right foot", move(right_foot_modifiers), foot_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, {ScreenRect{-3, -11, 1, 1}, ScreenRect{-2, -10, 2, 2}});
		Attributes::modifiers_t right_leg_modifiers;
		right_leg_modifiers.push_back(std::make_unique<Attributes::AgilityModifier>(40.0));
		auto right_leg = BodyPart::make<Leg>(*this, "Right leg", move(right_leg_modifiers), leg_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), false, {ScreenRect{-2, -2, 1, 1}, ScreenRect{-2, -3, 2, 7}});
		right_leg->attach(move(right_foot));

		auto torso = BodyPart::make<Torso>(*this, "Torso", Attributes::modifiers_t{}, torso_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), true, {ScreenRect{-3, 6, 7, 4}, ScreenRect{-2, 2, 5, 4}, ScreenRect{-1, -2, 3, 1}});
		torso->attach(move(left_arm));
		torso->attach(move(right_arm));
		torso->attach(move(left_leg));
		torso->attach(move(right_leg));

		auto head = BodyPart::make<Head>(*this, "Head", Attributes::modifiers_t{}, head_vitality, Protection::zero(), Resistance::zero(), Vulnerability::zero(), true, {ScreenRect{-1, 10, 3, 4}});
		head->attach(move(torso));

		return Body{*this, move(head)};
	}
}
