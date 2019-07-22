//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "being_widget.hpp"
#include "object_widget.hpp"
#include "tile_widget.hpp"

#include "animation/sprite_animation.hpp"
#include "rsrc/world_renderer.hpp"
#include "utility/reference.hpp"
#include "world/coordinates.hpp"

#include <optional>
#include <unordered_map>

namespace ql {
	namespace effects {
		struct effect;
	}
	namespace rsrc {
		struct entity;
		struct fonts;
		struct particle;
		struct tile;
	}
	struct being;
	struct entity;
	struct game;
	struct object;
	struct tile;
	struct world_view;

	//! Handles interaction with the world, as the player sees it.
	struct world_widget : widget {
		//! @param world_view The initial world view to render.
		world_widget( //
			world_view const& world_view,
			rsrc::world_renderer resources,
			rsrc::entity const& entity_resources,
			rsrc::fonts const& fonts,
			rsrc::tile const& tile_resources,
			rsrc::particle const& particle_resources);

		//! Updates the world renderer's world view.
		//! @param world_view The new world view to render.
		//! @param effects Any newly perceived effects to render.
		void render_view(world_view const& view, std::vector<effects::effect> const& effects);

		//! Advances animations by @p elapsed_time.
		void update(sec elapsed_time);

		//! Draws the visible terrain.
		void draw_terrain(sf::RenderTarget& target, sf::RenderStates states) const;

		//! Draws the visible entities in the world.
		void draw_entities(sf::RenderTarget& target, sf::RenderStates states) const;

		//! Draws visualizations of any active effects in the world.
		void draw_effects(sf::RenderTarget& target, sf::RenderStates states) const;

		//! Causes the tiles for which @p predicate returns true to be highlighted.
		void set_highlight_predicate(std::function<bool(region_tile::point)> predicate);

		//! Clears the current highlight predicate so that no tiles are highlighted.
		void clear_highlight_predicate();

		void render_effect(effects::effect const& effect);

	private:
		rsrc::world_renderer _resources;
		rsrc::entity const& _entity_resources;
		rsrc::fonts const& _fonts;
		rsrc::tile const& _tile_resources;
		rsrc::particle const& _particle_resources;

		std::reference_wrapper<world_view const> _world_view;

		std::unordered_map<ent, tile_widget> _tile_widgets;
		std::unordered_map<ent, being_widget> _being_widgets;
		std::unordered_map<ent, object_widget> _object_widgets;

		std::vector<uptr<animation>> _effect_animations;

		std::optional<std::function<bool(region_tile::point)>> _highlight_predicate;

		void render_terrain(world_view const& view);
		void render_entities(world_view const& view);

		animation& cache_tile_animation(tile const& tile);
		animation& cache_entity_animation(entity const& entity);
		animation& get_animation(ent entity_id) const;
	};
}
