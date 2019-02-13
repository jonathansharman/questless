//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "campfire.hpp"
#include "corpse.hpp"
#include "item_box.hpp"

#include "utility/id.hpp"
#include "utility/visitation.hpp"
#include "world/coordinates.hpp"

namespace ql {
	//! An inanimate entity.
	struct object {
		id<object> const id;

		location location;

		std::variant<campfire, corpse, item_box> subtype;

		double transparency() const {
			return match(subtype, [](auto const& subtype) { return subtype.transparency(); });
		}

		//! Whether the object blocks the movement of other entities.
		bool blocks_movement() const {
			return match(subtype, [](auto const& subtype) { return subtype.blocks_movement(); });
		}

		//! Advances this object in time by @p elapsed.
		void update(tick elapsed) {
			return match(subtype, [elapsed](auto& subtype) { subtype.update(elapsed); });
		}
	};
}
