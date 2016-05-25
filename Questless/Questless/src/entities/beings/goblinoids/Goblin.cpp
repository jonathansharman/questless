/**
* @file    Goblin.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Goblin class.
*/

#include "entities/beings/goblinoids/Goblin.h"
#include "entities/beings/Agent.h"
#include "entities/beings/BodyPart.h"

namespace questless
{
	Goblin::Goblin(std::function<std::unique_ptr<Agent>(Being&)> agent_factory, id_t id)
		: Being(std::move(agent_factory), id, make_body(), Attributes{ATTRIBUTE_PARAMS(GoblinK)})
	{}

	Goblin::Goblin(std::istream& in) : Being(in, make_body()) {}

	void Goblin::serialize(std::ostream& out) const
	{
		Being::serialize(out);

		out << std::endl;
	}

	BodyPart::ptr Goblin::make_body()
	{
		using std::move;

		auto left_hand = BodyPart::make<Hand>("Left hand", Attributes{}, false, {{4, -2, 2, 2}});
		auto left_arm = BodyPart::make<Arm>("Left arm", Attributes{}, false, {{4, 6, 1, 1}, {4, 5, 2, 7}});
		left_arm->attach(move(left_hand));

		auto right_hand = BodyPart::make<Hand>("Right hand", Attributes{}, false, {{-5, -2, 2, 2}});
		auto right_arm = BodyPart::make<Arm>("Right arm", Attributes{}, false, {{-4, 6, 1, 1}, {-5, 5, 2, 7}});
		right_arm->attach(move(right_hand));

		auto left_foot = BodyPart::make<Foot>("Left foot", Attributes{}, false, {{3, -11, 1, 1}, {1, -10, 2, 2}});
		auto left_leg = BodyPart::make<Leg>("Left leg", Attributes{}, false, {{2, -2, 1, 1}, {1, -3, 2, 7}});
		left_leg->attach(move(left_foot));

		auto right_foot = BodyPart::make<Foot>("Right foot", Attributes{}, false, {{-3, -11, 1, 1}, {-2, -10, 2, 2}});
		auto right_leg = BodyPart::make<Leg>("Right leg", Attributes{}, false, {{-2, -2, 1, 1}, {-2, -3, 2, 7}});
		right_leg->attach(move(right_foot));

		auto torso = BodyPart::make<Torso>("Torso", Attributes{}, false, {{-3, 6, 7, 4}, {-2, 2, 5, 4}, {-1, -2, 3, 1}});
		torso->attach(move(left_arm));
		torso->attach(move(right_arm));
		torso->attach(move(left_leg));
		torso->attach(move(right_leg));

		auto head = BodyPart::make<Head>("Head", Attributes{}, false, {{-1, 10, 3, 4}, {-2, 9, 5, 1}});
		head->attach(move(torso));

		return head;
	}
}
