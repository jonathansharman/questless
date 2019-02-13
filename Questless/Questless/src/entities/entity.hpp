//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "beings/being.hpp"
#include "objects/object.hpp"

#include "utility/visitation.hpp"

namespace ql {
	struct region;
	struct section;

	//! Things that can exist on the world map: beings or objects.
	struct entity {
		std::variant<being, object> value;

		//! The location of this entity.
		location location() const {
			return match(value, [](auto const& entity) { return entity.location; });
		};

		//! Advances this entity in time by @p elapsed.
		void update(tick elapsed) {
			match(value, [elapsed](auto& entity) { entity.update(elapsed); });
		}

		//! The proportion of light or visual information this entity allows through, in the range [0, 1].
		double transparency() const {
			return match(value, [](auto& entity) { return entity.transparency(); });
		}
	};
}
