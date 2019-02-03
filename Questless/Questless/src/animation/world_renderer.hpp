//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "sprite_animation.hpp"

#include "effects/effect.hpp"
#include "rsrc/entity.hpp"
#include "rsrc/tile.hpp"
#include "rsrc/world_renderer.hpp"
#include "utility/id.hpp"
#include "utility/reference.hpp"
#include "world/coordinates.hpp"

#include <optional>
#include <unordered_map>
#include <variant>

namespace ql {
	namespace rsrc {
		struct fonts;
	}
	struct being;
	struct entity;
	struct game;
	struct object;
	struct tile;
	struct world_view;

	//! Draws the elements of the world that the player can see.
	struct world_renderer {
		//! @param world_view The initial world view to render.
		world_renderer(rsrc::fonts const& fonts, world_view const& world_view);

		//! Updates the world renderer's world view.
		//! @param world_view The new world view to render.
		//! @param effects Any newly perceived effects to render.
		void update_view(world_view const& world_view, std::vector<effects::effect> const& effects);

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

	private:
		using entity_id_var_t = std::variant<id<being>, id<object>>;
		using entity_cref_var_t = std::variant<cref<being>, cref<object>>;
		using entity_cptr_var_t = std::variant<being const*, object const*>;

		rsrc::world_renderer _resources;
		rsrc::entity _entity_resources;
		rsrc::tile _tile_resources;
		rsrc::fonts const& _fonts;

		static entity_id_var_t get_entity_id_var(entity_cref_var_t entity);
		static entity_cref_var_t get_entity_cref_var(entity_id_var_t id);
		static entity const* get_entity_cptr(entity_id_var_t id);

		world_view const* _world_view;

		std::unordered_map<id<tile>, uptr<animation>> _tile_animations;
		std::unordered_map<entity_id_var_t, uptr<animation>> _entity_animations;

		std::vector<uptr<animation>> _effect_animations;

		std::optional<std::function<bool(region_tile::point)>> _highlight_predicate;

		animation& cache_tile_animation(tile const& tile);
		animation& cache_entity_animation(entity_id_var_t entity);
		animation& get_animation(entity_id_var_t entity) const;
	};
}
