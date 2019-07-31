//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "entity.hpp"
#include "fonts.hpp"
#include "item.hpp"
#include "particle.hpp"
#include "tile.hpp"

namespace ql::rsrc {
	//! Contains resources for the world renderer.
	struct hud {
		entity const& entity;
		fonts const& fonts;
		item const& item;
		particle const& particle;
		tile const& tile;
	};
}
