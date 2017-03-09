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
		, Vitality vitality
		, Weight weight
		, Protection protection
		, Resistance resistance
		, Vulnerability vulnerability
		, std::vector<ScreenRect> regions
		)
		: health{vitality}
		, _owner{owner}
		, _name{std::move(name)}
		, _enabled{true}
		, _vitality{vitality}
		, _weight{weight}
		, _protection{protection}
		, _resistance{resistance}
		, _vulnerability{vulnerability}
		, _regions{std::move(regions)}
	{
		health.set_mutator(health_mutator(), false);
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
		health += _owner.stats.health_regen * _vitality / _owner.stats.vitality;
	}

	void BodyPart::take_damage(Damage& damage, boost::optional<BeingId> source_id)
	{
		_owner.take_damage(damage, this, source_id);
	}
	
	std::function<void(double&, double const&)> BodyPart::health_mutator()
	{
		return [this](double& health, double const& new_health)
		{
			health = std::clamp(new_health, 0.0, _vitality);
		};
	}
}
