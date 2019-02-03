//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animators.hpp"

#include "animation.hpp"
#include "still_image.hpp"

#include "magic/spell.hpp"
#include "rsrc/spell.hpp"
#include "rsrc/tile.hpp"
#include "utility/utility.hpp"
#include "utility/visitation.hpp"
#include "world/tile.hpp"

namespace ql {
	uptr<animation> animate(rsrc::tile const& resources, tile const& tile) {
		return match(tile.terrain, //
			[&](dirt const&) { return umake<still_image>(resources.txtr.dirt); },
			[&](edge const&) {
				//! @todo Edge tiles should simply not be drawn.
				return umake<still_image>(resources.txtr.blank);
			},
			[&](grass const&) { return umake<still_image>(resources.txtr.grass); },
			[&](sand const&) { return umake<still_image>(resources.txtr.sand); },
			[&](snow const&) { return umake<still_image>(resources.txtr.snow); },
			[&](stone const&) { return umake<still_image>(resources.txtr.stone); },
			[&](water const&) { return umake<still_image>(resources.txtr.water); });
	}

	uptr<animation> animate(rsrc::spell const& resources, magic::spell const& spell) {
		return match(spell, //
			[&](magic::telescope const&) { return umake<still_image>(resources.txtr.telescope); },
			[&](magic::heal const&) { return umake<still_image>(resources.txtr.heal); },
			[&](magic::shock const&) { return umake<still_image>(resources.txtr.shock); },
			[&](magic::teleport const&) { return umake<still_image>(resources.txtr.teleport); });
	}
}
