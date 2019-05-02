//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "arrow.hpp"

#include "items/common_actions.hpp"

#include "animation/still_image.hpp"
#include "rsrc/item.hpp"
#include "utility/utility.hpp"

namespace ql {
	arrow::arrow(ql::id<item> id) : item{id, "Arrow", 0.1_mass} {}

	uptr<animation> arrow::make_animation(rsrc::item const& resources) const {
		return umake<still_image>(resources.arrow);
	};

	return make_uptr_vector<action>(umake<drop>(*this), umake<toss>(*this))
}
