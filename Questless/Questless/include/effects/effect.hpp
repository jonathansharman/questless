//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "effect_visitor.hpp"
#include "damage/group.hpp"
#include "utility/id.hpp"
#include "world/coordinates.hpp"

namespace ql
{
	class being;

	//! An in-game effect perceivable by beings, such as light and sound stimuli.
	class effect : public element<effect_subtype_list>
	{
	public:
		//! @param origin The coordinates of the effect's origin.
		effect(region_tile::point origin) : _origin{origin} {}

		virtual ~effect() = default;

		//! The maximum distance from the origin at which this effect may be perceived.
		virtual int range() const = 0;

		region_tile::point origin() const { return _origin; }
	private:
		region_tile::point _origin;
	};

	DEFINE_ELEMENT_BASE(effect, effect)

	//! @todo Eventually move effect subtypes to individual header files.

	class eagle_eye_effect : public effect_base<eagle_eye_effect>
	{
	public:
		id<being> const caster_id;

		//! @param origin The coordinates of the effect's origin.
		//! @param caster_id The ID of the caster.
		eagle_eye_effect(region_tile::point origin, id<being> caster_id)
			: effect_base<eagle_eye_effect>{origin}, caster_id{caster_id}
		{}

		int range() const final { return 3; }
	};

	class injury_effect : public effect_base<injury_effect>
	{
	public:
		dmg::group const damage;
		id<being> target_id;
		std::optional<id<being>> opt_source_id;

		//! @param origin The coordinates of the effect's origin.
		//! @param damage The damage dealt.
		//! @param target_id The ID of the injured being.
		//! @param opt_source_id The ID of the being who caused the injury or nullopt if there is none.
		injury_effect(region_tile::point origin, dmg::group damage, id<being> target_id, std::optional<id<being>> opt_source_id)
			: effect_base<injury_effect>{origin}, damage{damage}, target_id{target_id}, opt_source_id{opt_source_id}
		{}

		int range() const final { return 7; }
	};

	class lightning_bolt_effect : public effect_base<lightning_bolt_effect>
	{
	public:
		using effect_base<lightning_bolt_effect>::effect_base;

		int range() const final { return 10; }
	};
}
