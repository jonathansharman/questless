//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/body_part.hpp"

#include "entities/beings/being.hpp"
#include "game.hpp"

using std::string;
using std::vector;

using namespace units;

namespace ql {
	body_part::body_part
		( ql::id<being> owner_id
		, ql::vitality vitality
		, ql::weight weight
		, vector<uptr<attachment>> attachments
		, ql::id<body_part> id
		)
		: id{id}
		, vitality{vitality}
		, health{vitality, [] { return 0.0; }, [this] { return this->vitality; }}
		, bleeding{0.0, [] { return 0.0; }, [this] { return this->vitality; }}
		, _owner_id{owner_id}
		, _attachments{std::move(attachments)}
		, _enabled{true}
		, _weight{weight}
	{}

	void body_part::update() {
		being& owner = the_game().beings.ref(_owner_id);

		health += owner.stats.health_regen * vitality;

		bleeding -= owner.stats.health_regen * vitality;
	}

	void body_part::take_damage(dmg::group& damage, std::optional<ql::id<being>> source_id) {
		if (being* owner = the_game().beings.ptr(_owner_id)) {
			owner->take_damage(damage, *this, source_id);
		}
	}

	void body_part::heal(double amount, std::optional<ql::id<being>> opt_source_id) {
		being& owner = the_game().beings.ref(_owner_id);
		owner.heal(amount, *this, opt_source_id);
	}

	void body_part::generate_attached_parts() {
		for (auto& attachment : _attachments) {
			attachment->part = attachment->default_part(_owner_id);
			attachment->part->generate_attached_parts();
		}
	}
}
