//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "Object.h"
#include "entities/beings/Being.h"

namespace questless
{
	//! A simple source of heat and light.
	class Campfire : public ObjectBase<Campfire>
	{
	public:
		Campfire(Region& region, RegionTileCoords coords, Id<Object> id = Id<Object>::make());
		Campfire(std::istream& in);

		virtual EntityClass entity_class() const { return EntityClass::Campfire; }

		double transparency() const final { return 1.0; }

		bool blocks_movement() const final { return true; }
	};
}
