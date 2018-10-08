//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/objects/campfire.hpp"
#include "game.hpp"
#include "world/light_source.hpp"
#include "world/region.hpp"

namespace ql {
	//! @todo How to keep the light source in sync with the movement of the campfire, if it were to move?

	campfire::campfire(ql::region& region, region_tile::point coords, ql::id<object> id) : object_base<campfire>{id} {
		//! @todo region and coords hide class members. (Related to the above TODO concerning light sources and campires.)
		auto light_source = umake<ql::light_source>(coords, 100.0);
		region.add(*light_source);
		the_game().light_sources.add(std::move(light_source));
	}
}
