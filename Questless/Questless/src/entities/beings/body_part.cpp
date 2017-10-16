//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/body_part.h"
#include "entities/beings/being.h"

using std::string;
using std::vector;

using namespace units;

namespace ql
{
	body_part::body_part
		( being& owner
		, std::string name
		, ql::vitality vitality
		, ql::weight weight
		, dmg::protect protection
		, dmg::resist resistance
		, dmg::vuln vulnerability
		, std::vector<screen_space::box> regions
		, ql::id<body_part> id
		)
		: id{id}
		, health {vitality, [] { return 0.0; }, [this]() { return _vitality; }}
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
		for (auto& region : _regions) {
			left(region) *= 5;
			top(region) *= 5;
			
			top(region) = -top(region);

			width(region) *= 5;
			height(region) *= 5;

			++width(region);
			++height(region);
		}
	}

	void body_part::update()
	{
		health += _owner.stats.health_regen * _vitality / _owner.stats.vitality;
	}

	void body_part::take_damage(dmg::group& damage, std::optional<ql::id<being>> source_id)
	{
		_owner.take_damage(damage, this, source_id);
	}
	
	std::function<void(double&, double const&)> body_part::health_mutator()
	{
		return [this](double& health, double const& new_health)
		{
			health = std::clamp(new_health, 0.0, _vitality);
		};
	}
}
