//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entities/beings/body_part.hpp"

#include <memory>
#include <vector>

namespace ql {
	struct being;

	//! A being's body, which is composed of a tree of body parts.
	struct body {
		//! The amount of blood in this body.
		lazy_bounded<ql::blood> blood;

		//! @param owner The ID of the being that owns this body.
		//! @param root The root of the body parts tree.
		body(id<being> owner_id, body_part root);

		//! The root body part, to which all other body parts are attached.
		body_part const& root() const {
			return _root;
		}
		//! The root body part, to which all other body parts are attached.
		body_part& root() {
			return _root;
		}

		//! The cumulative vitality of all parts of this body.
		ql::health total_vitality() const {
			return _total_vitality;
		}

		std::vector<cref<body_part>> const& parts() const {
			return _c_parts;
		}
		std::vector<ref<body_part>> const& parts() {
			return _parts;
		}

		//! The body part on this body with the given ID or nullptr if none exists.
		body_part const* find_part(id<body_part> id) const;
		//! The body part on this body with the given ID or nullptr if none exists.
		body_part* find_part(id<body_part> id);

		//! Advances this body and all its parts by @p elapsed.
		void update(tick elapsed);

	private:
		id<being> _owner_id;

		body_part _root;

		std::vector<ref<body_part>> _parts;
		std::vector<cref<body_part>> _c_parts;

		// Cumulative attributes
		ql::health _total_vitality;
	};

	struct animation;
	uptr<animation> get_animation(body const& body);
}
