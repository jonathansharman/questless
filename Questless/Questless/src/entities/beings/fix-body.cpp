//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/beings/body.hpp"

#include <algorithm>
#include <memory>
#include <queue>

#include "entities/beings/being.hpp"
#include "entities/beings/body_part.hpp"
#include "game.hpp"
#include "utility/utility.hpp"

using namespace units;

namespace ql {
	body::body(being& owner, body_part root)
		: blood
			{ 0.0_blood
			, [] { return 0.0_blood; }
			, [&owner = owner] { return owner.body.total_vitality() * body_part::blood_per_vitality; }
			}
		, _owner{owner}
		, _root{std::move(root)}
		, _total_vitality{0.0_hp}
	{
		// Walk the parts tree to build the parts lists and compute cumulative values.
		std::queue<ref<body_part>> work_list;
		work_list.push(_root);
		while (!work_list.empty()) {
			body_part& part = work_list.front();
			_parts.push_back(part);
			_c_parts.push_back(part);

			// Add part values to totals.
			_total_vitality += part.stats.a.vitality.value();

			// Remove current part from work list and add its children.
			work_list.pop();
			for (auto const& attachment : part.attachments) {
				if (attachment->part) {
					work_list.push(*attachment->part);
				}
			}
		}

		// Start with maximum blood.
		blood = _total_vitality * body_part::blood_per_vitality;
	}

	body_part* body::find_part(id<body_part> id) {
		for (body_part& part : _parts) {
			if (part.id == id) {
				return &part;
			}
		}
		return nullptr;
	}
	body_part const* body::find_part(id<body_part> id) const {
		for (body_part& part : _parts) {
			if (part.id == id) {
				return &part;
			}
		}
		return nullptr;
	}

	void body::update(tick elapsed) {
		// Update cumulative values.
		_total_vitality = 0.0_hp;
		for (body_part const& part : _parts) {
			_total_vitality += part.stats.a.vitality.value();
		}

		// Bleed and regenerate blood.
		for (body_part const& part : _parts) {
			blood += (_owner.stats.regen.value() * part.stats.a.vitality.value() * body_part::blood_per_vitality - part.bleeding.value()) * elapsed;
		}

		// Update parts.
		for (body_part& part : _parts) {
			part.update(elapsed);
		}
	}
}
