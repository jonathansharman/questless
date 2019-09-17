//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "entity.hpp"
#include "fonts.hpp"
#include "item.hpp"
#include "particle.hpp"
#include "spell.hpp"
#include "tile.hpp"

namespace ql::rsrc {
	//! Contains resources for the world renderer.
	struct hud {
		fonts const& fonts;

		entity entity;
		item item;
		particle particle;
		spell spell;
		tile tile;
	};
}
