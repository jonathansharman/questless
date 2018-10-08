//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "effect_visitor.hpp"

#include "damage/group.hpp"
#include "utility/id.hpp"
#include "utility/quantities.hpp"
#include "world/coordinates.hpp"

#include <optional>

namespace ql {
	class being;
	class body_part;

	//! An in-game effect perceivable by beings, such as light and sound stimuli.
	class effect : public element<effect_subtype_list> {
	public:
		//! @param origin The coordinates of the effect's origin.
		effect(region_tile::point origin) : _origin{origin} {}

		virtual ~effect() = default;

		//! The maximum distance from the origin at which this effect may be perceived.
		virtual span range() const = 0;

		region_tile::point origin() const { return _origin; }
	private:
		region_tile::point _origin;
	};

	DEFINE_ELEMENT_BASE(effect, effect)

	//! @todo Eventually move effect subtypes to individual header files.

	class arrow_attack_effect : public effect_base<arrow_attack_effect> {
	public:
		region_tile::point const target;

		//! @param origin The coordinates of the effect's origin.
		//! @param target The coordinates of the attack target.
		arrow_attack_effect(region_tile::point origin, region_tile::point target)
			: effect_base<arrow_attack_effect>{origin}
			, target{target}
		{}

		span range() const final { return 2 * (target - origin()).length(); }
	};

	class eagle_eye_effect : public effect_base<eagle_eye_effect> {
	public:
		id<being> const caster_id;

		//! @param origin The coordinates of the effect's origin.
		//! @param caster_id The ID of the caster.
		eagle_eye_effect(region_tile::point origin, id<being> caster_id)
			: effect_base<eagle_eye_effect>{origin}
			, caster_id{caster_id}
		{}

		span range() const final { return 3_span; }
	};

	class injury_effect : public effect_base<injury_effect> {
	public:
		dmg::group const damage;
		id<being> target_being_id;
		id<body_part> target_part_id;
		std::optional<id<being>> opt_source_id;

		//! @param origin The coordinates of the effect's origin.
		//! @param damage The damage dealt.
		//! @param target_being_id The ID of the injured being.
		//! @param target_part_id The ID of the injured part.
		//! @param opt_source_id The ID of the being who caused the injury or nullopt if there is none.
		injury_effect
			( region_tile::point origin
			, dmg::group damage
			, id<being> target_being_id
			, id<body_part> target_part_id
			, std::optional<id<being>> opt_source_id
			)
			: effect_base<injury_effect>{origin}
			, damage{damage}
			, target_being_id{target_being_id}
			, target_part_id{target_part_id}
			, opt_source_id{opt_source_id}
		{}

		span range() const final { return 7_span; }
	};

	class lightning_bolt_effect : public effect_base<lightning_bolt_effect> {
	public:
		using effect_base<lightning_bolt_effect>::effect_base;

		span range() const final { return 10_span; }
	};
}
