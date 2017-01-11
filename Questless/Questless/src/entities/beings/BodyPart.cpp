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

using namespace units;

namespace questless
{
	BodyPart::BodyPart
		( Being& owner
		, std::string name
		, std::vector<Modifier::ptr> modifiers
		, double vitality
		, Protection protection
		, Resistance resistance
		, Vulnerability vulnerability
		, bool vital
		, std::vector<ScreenRect> regions
		)
		: _owner{owner}
		, _name{std::move(name)}
		, _modifiers{std::move(modifiers)}
		, _health{vitality}
		, _vitality{vitality}
		, _protection{protection}
		, _resistance{resistance}
		, _vulnerability{vulnerability}
		, _vital{vital}
		, _regions{std::move(regions)}
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

	void BodyPart::take_damage(Damage& damage, boost::optional<BeingId> source_id)
	{
		_owner.take_damage(damage, this, source_id);
	}
}
