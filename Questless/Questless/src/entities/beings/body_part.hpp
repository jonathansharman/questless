//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "agents/action.hpp"
#include "damage/group.hpp"
#include "stats/part.hpp"
#include "utility/id.hpp"
#include "utility/lazy_bounded.hpp"
#include "utility/reference.hpp"
#include "utility/utility.hpp"

#include "units/view_space.hpp"
#include "utility/quantities.hpp"

#include <nlohmann/json_fwd.hpp>

#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

namespace ql {
	struct attachment;
	class being;
	class item;

	//! A being's body part.
	class body_part {
	public:
		enum class tag : int { head = 0, torso, arm, hand, leg, foot, wing, tail };

		static constexpr auto blood_per_vitality = 1.0_blood / 1.0_hp;

		ql::id<body_part> id;

		//! @param owner_id The ID of the being that owns this body part.
		ql::id<being> owner_id;

		std::unordered_set<tag> tags;

		//! The player-visisble name of the body part.
		std::string name;

		//! This body part's contribution to its owner's stats.
		stats::part stats{};

		//! Whether this body part is vital to its being. If true, the being usually dies when this body part is disabled.
		vital vital = ql::vital{false};

		//! Whether this body part is currently functioning. Parts can be disabled by damage or other effects.
		enabled enabled = ql::enabled{true};

		lazy_bounded<ql::health> health
			{ 0.0_hp
			, [] { return 0.0_hp; }
			, [this] { return this->stats.a.vitality.value(); }
			};

		//! The rate at which this body part is losing blood.
		lazy_bounded<blood_per_tick> bleeding
			{ 0.0_blood_per_tick
			, [] { return 0.0_blood_per_tick; }
			, [this] { return this->stats.a.vitality.value() * blood_per_vitality / 1.0_tick; }
			};

		//! The region this body part occupies, for collision and display.
		units::view_space::polygon hitbox;

		//! The draw layer, with smaller-numbered layers drawn first (i.e. in the background).
		int layer = 0;

		//! The ID of the item equipped to this body or nullopt if none.
		std::optional<ql::id<item>> equipped_item_id = std::nullopt;

		//! The body part attachment points on this body part.
		std::vector<uptr<attachment>> attachments;

		body_part(ql::id<being> owner_id, ql::id<body_part> id) : id{id}, owner_id{owner_id} {}

		//! Loads a body part from @filepath.
		body_part(char const* filepath, ql::id<being> owner_id, ql::id<body_part> id = ql::id<body_part>::make());

		friend void to_json(nlohmann::json& j, body_part const& part);

		friend void from_json(nlohmann::json const& j, body_part& part);

		//! Advances the body part one time unit.
		void update();

		//! Causes the body part to take damage from the specified source being.
		//! @param damage Damage group to be applied to this being.
		//! @param opt_source_id The ID of the being which caused the damage, if any.
		void take_damage(dmg::group& damage, std::optional<ql::id<being>> opt_source_id);

		//! Causes the body part to be healed by the specified source being.
		//! @param amount Health to be restored to this being.
		//! @param opt_source_id The ID of the being which caused the healing, if any.
		void heal(ql::health amount, std::optional<ql::id<being>> opt_source_id);

		//! Recursively generates each missing attached body part using default parts.
		void generate_attached_parts();
	};

	//! An attachment point on a body part.
	struct attachment {
		//! @param owner_id The ID of the being that owns this body part attachment.
		ql::id<being> owner_id;

		//! The body part currently attached here or nullopt if there is no attached part.
		std::optional<body_part> part;

		//! The offset from the parent part's origin to the child part's origin.
		units::view_space::vector offset;

		//! The rotation of the attached part about the attachment point.
		units::view_space::radians rotation;

		//! The resource filepath to the default part for this attachment.
		std::string default_part;

		//! Creates an instance of the body part that attaches here by default.
		//! @param owner_id The ID of the being that owns the new body part.
		body_part make_default() {
			return body_part{default_part.c_str(), owner_id};
		}

		friend void to_json(nlohmann::json& j, attachment const& attachment);

		friend void from_json(nlohmann::json const& j, attachment& attachment);
	};
}
