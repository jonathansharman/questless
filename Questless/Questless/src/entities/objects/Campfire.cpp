//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "entities/objects/Campfire.h"
#include "Game.h"
#include "world/LightSource.h"
#include "world/Region.h"

namespace questless
{
	//! @todo How to keep the light source in sync with the movement of the campfire, if it were to move?

	Campfire::Campfire(Region& region, RegionTileCoords coords, Id<Object> id) : ObjectBase<Campfire>{id}
	{
		auto light_source = std::make_unique<LightSource>(coords, 100.0);
		region.add(*light_source);
		game().light_sources.add(std::move(light_source));
	}

	Campfire::Campfire(std::istream& in) : ObjectBase<Campfire>{in}
	{
		//! @todo Light source.
	}
}
