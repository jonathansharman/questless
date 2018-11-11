//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "agents/action.hpp"
#include "damage/group.hpp"
#include "quantities/quantities.hpp"
#include "stats/part.hpp"
#include "utility/id.hpp"
#include "utility/lazy_bounded.hpp"
#include "utility/reference.hpp"
#include "utility/utility.hpp"

#include "units/view_space.hpp"

#include <cereal/access.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/optional.hpp>
#include <cereal/types/unordered_set.hpp>
#include <cereal/types/vector.hpp>

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

		//! The player-visisble name of this body part.
		std::string name;

		//! The tags that determine what kind of part this is.
		std::unordered_set<tag> tags;

		//! This body part's contribution to its owner's stats.
		stats::part stats{};

		//! Whether this body part is vital to its being. If true, the being usually dies when this body part is disabled.
		vitalness vitalness = vitalness::nonvital;
		constexpr bool vital() const { return vitalness == vitalness::vital; }
		constexpr bool nonvital() const { return vitalness == vitalness::nonvital; }

		//! Whether this body part is currently functioning. Parts can be disabled by damage or other effects.
		ableness ableness = ableness::enabled;
		constexpr bool enabled() const { return ableness == ableness::enabled; }
		constexpr bool disabled() const { return ableness == ableness::disabled; }

		lazy_bounded<ql::health> health
			{ 0.0_hp
			, [] { return 0.0_hp; }
			, [this] { return this->stats.a.vitality.value(); }
			};

		//! The rate at which this body part is losing blood.
		lazy_bounded<blood_per_tick> bleeding
			{ 0.0_blood_per_tick
			, [] { return 0.0_blood_per_tick; }
			, [this] { return this->stats.a.vitality.value() * blood_per_vitality / 1_tick; }
			};

		//! The region this body part occupies, for collision and display.
		spaces::view::polygon hitbox;

		//! The draw layer, with smaller-numbered layers drawn first (i.e. in the background).
		int layer = 0;

		//! The ID of the item equipped to this body or nullopt if none.
		std::optional<ql::id<item>> equipped_item_id = std::nullopt;

		//! The body part attachment points on this body part.
		std::vector<uptr<attachment>> attachments;

		body_part() = default;

		//! Loads a body part from @filepath.
		body_part(char const* filepath, ql::id<being> owner_id, ql::id<body_part> id = ql::id<body_part>::make());

		template <typename Archive>
		friend constexpr std::string save_minimal(Archive const&, tag const& t) {
			switch (t) {
				case tag::head: return "head";
				case tag::torso: return "torso";
				case tag::arm: return "arm";
				case tag::hand: return "hand";
				case tag::leg: return "leg";
				case tag::foot: return "foot";
				case tag::wing: return "wing";
				default: return "tail";
			}
		}

		template <typename Archive>
		friend void load_minimal(Archive const&, tag& t, std::string const& name) {
			t = [&name] {
				if (name == "head") return tag::head;
				else if (name == "torso") return tag::torso;
				else if (name == "arm") return tag::arm;
				else if (name == "hand") return tag::hand;
				else if (name == "leg") return tag::leg;
				else if (name == "foot") return tag::foot;
				else if (name == "wing") return tag::wing;
				else return tag::tail;
			}();
		}

		template <typename Archive>
		void save(Archive& archive) const {
			archive
				( CEREAL_NVP(id)
				, CEREAL_NVP(owner_id)
				, CEREAL_NVP(name)
				, CEREAL_NVP(tags)
				, CEREAL_NVP(stats)
				, CEREAL_NVP(vitalness)
				, CEREAL_NVP(ableness)
				, CEREAL_NVP(layer)
				, CEREAL_NVP(hitbox)
				, CEREAL_NVP(equipped_item_id)
				, CEREAL_NVP(attachments)
				);
		}

		template <typename Archive>
		void load(Archive& archive) {
			archive
				( CEREAL_NVP(id)
				, CEREAL_NVP(owner_id)
				, CEREAL_NVP(name)
				, CEREAL_NVP(tags)
				, CEREAL_NVP(stats)
				, CEREAL_NVP(vital)
				, CEREAL_NVP(enabled)
				, CEREAL_NVP(layer)
				, CEREAL_NVP(hitbox)
				, CEREAL_NVP(equipped_item_id)
				, CEREAL_NVP(attachments)
				);
		}

		//! Advances the body part by @p elapsed.
		void update(tick elapsed);

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
		spaces::view::vector offset;

		//! The rotation of the attached part about the attachment point.
		spaces::view::radians rotation;

		//! The resource filepath to the default part for this attachment.
		std::string default_part;

		//! Creates an instance of the body part that attaches here by default.
		//! @param owner_id The ID of the being that owns the new body part.
		body_part make_default() {
			return body_part{default_part.c_str(), owner_id};
		}

		template <typename Archive>
		void save(Archive& archive) const {
			archive(CEREAL_NVP(owner_id), CEREAL_NVP(part), CEREAL_NVP(offset), CEREAL_NVP(rotation), CEREAL_NVP(default_part));
		}

		template <typename Archive>
		void load(Archive& archive) {
			archive(CEREAL_NVP(owner_id), CEREAL_NVP(part), CEREAL_NVP(offset), CEREAL_NVP(rotation), CEREAL_NVP(default_part));
		}
	};
}
