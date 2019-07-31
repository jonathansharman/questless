//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "scene.hpp"

#include "rsrc/entity.hpp"
#include "rsrc/item.hpp"
#include "rsrc/particle.hpp"
#include "rsrc/tile.hpp"
#include "ui/hud.hpp"
#include "ui/window_widget.hpp"
#include "utility/reference.hpp"

namespace ql::scenes {
	//! The scene for an active game.
	struct playing : scene {
		playing(view::vector window_size, rsrc::fonts const& fonts, ent region_id);

	private:
		// Resources

		rsrc::entity _entity_resources;
		rsrc::item _item_resources;
		rsrc::particle _particle_resources;
		rsrc::tile _tile_resources;

		// Entities

		ent _region_id;
		ent _player_id;

		// UI

		window_widget _root;
		hud _hud;

		update_result scene_subupdate(sec elapsed_time, std::vector<sf::Event>& events) final;

		void scene_subdraw(sf::RenderTarget& target, sf::RenderStates states) const final;
	};
}
