//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>
#include <vector>

#include "entities/beings/body_part.hpp"
#include "units/window_space.hpp"

namespace ql {
	class being;

	//! A being's body, which is composed of a tree of body parts.
	class body {
	public:
		//! The amount of blood in this body.
		lazy_bounded<ql::blood> blood;

		//! @param owner The being that owns this body.
		//! @param root The root of the body parts tree.
		body(being& owner, body_part root);

		body(body&&) = default;

		//! The root body part, to which all other body parts are attached.
		body_part const& root() const { return _root; }
		//! The root body part, to which all other body parts are attached.
		body_part& root() { return _root; }

		//! The cumulative vitality of all parts of this body.
		ql::health total_vitality() const { return _total_vitality; }

		std::vector<cref<body_part>> const& parts() const { return _c_parts; }
		std::vector<ref<body_part>> const& parts() { return _parts; }

		//! The body part on this body with the given ID or nullptr if none exists.
		body_part* find_part(id<body_part> id);
		//! The body part on this body with the given ID or nullptr if none exists.
		body_part const* find_part(id<body_part> id) const;

		//! Advances this body and all its parts by @p elapsed.
		void update(tick elapsed);
	private:
		being& _owner; // Okay to store reference here. A body's lifetime is a subset of its owning being's lifetime.

		body_part _root;

		std::vector<ref<body_part>> _parts;
		std::vector<cref<body_part>> _c_parts;

		// Cumulative attributes
		ql::health _total_vitality;
	};
}
