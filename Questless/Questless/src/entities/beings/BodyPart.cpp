/**
* @file    BodyPart.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the BodyPart class.
*/

#include "entities/beings/BodyPart.h"
#include "entities/beings/Being.h"

using std::string;
using std::vector;

using namespace sdl;

namespace questless
{
	BodyPart::BodyPart(Being& owner, string name, double vitality, Attributes attributes, bool vital, vector<Rect> regions)
		: _owner{owner}
		, _name{std::move(name)}
		, _attributes{attributes}
		, _vital{vital}
		, _regions{std::move(regions)}
		, _health{vitality}
		, _vitality{vitality}
	{
		for (auto& region : _regions) {
			region.x *= 5;
			region.y *= 5;
			
			region.y = -region.y;

			region.w *= 5;
			region.h *= 5;

			++region.w;
			++region.h;
		}
	}

	void BodyPart::update()
	{
		gain_health(_owner.health_regen() * _vitality / _owner.vitality());
	}

	void BodyPart::gain_health(double amount)
	{
		_health += amount;
		if (_health > _vitality) { _health = _vitality; }
	}

	void BodyPart::lose_health(double amount)
	{
		_health -= amount;
		if (_health < 0) { _health = 0; }
	}

	void BodyPart::take_damage(Damage& damage, optional<BeingId> source_id)
	{
		// Apply damage to the owner.
		_owner.take_damage(damage, source_id);

		/// @todo What happens if the damage is mitigated inside one of the owner's before-/after-damage events?

		// Apply damage to the part.
		Damage damage_reduction = Damage::zero();
		for (Armor& armor : _armor) {
			damage_reduction += armor.apply(damage);
		}
		double applied_damage = (damage - damage_reduction).total() / (1.0 + Being::endurance_factor * _owner.endurance());
		lose_health(applied_damage);

		// Check for disability.
		if (health() <= 0) {
			/// @todo Disable part and, if vital, kill owner.
		}
	}
}
