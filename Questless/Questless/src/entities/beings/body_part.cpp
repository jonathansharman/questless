//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/body_part.hpp"

#include "entities/beings/being.hpp"
#include "game.hpp"
#include "utility/io.hpp"

using namespace units;

namespace ql {
	body_part::body_part(ent id, char const* filepath, ent owner_id) : id{id}, owner_id{owner_id} {
		load_from_json(filepath, *this);
	}

	void body_part::update(tick elapsed) {
		being& owner = the_game().beings.ref(owner_id);

		health += (owner.awake.value ? 1.0 : 1.5) * owner.stats.regen.value() * stats.a.vitality.value() * elapsed;

		bleeding -= owner.stats.regen.value() * stats.a.vitality.value() * blood_per_vitality * elapsed;
	}

	void body_part::take_damage(dmg::group& damage, std::optional<ent> source_id) {
		if (being* owner = the_game().beings.ptr(owner_id)) { owner->take_damage(damage, *this, source_id); }
	}

	void body_part::heal(ql::health amount, std::optional<ent> opt_source_id) {
		if (being* owner = the_game().beings.ptr(owner_id)) { owner->heal(amount, *this, opt_source_id); }
	}

	void body_part::generate_attached_parts() {
		for (auto& attachment : attachments) {
			attachment->part = attachment->make_default();
			attachment->part->generate_attached_parts();
		}
	}
}
