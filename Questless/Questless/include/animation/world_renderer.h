//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>
#include <unordered_map>
#include <variant>

#include "camera.h"
#include "effects/effect.h"
#include "entities/beings/world_view.h"
#include "entities/entity.h"
#include "sdl/renderable.h"
#include "sdl/renderer.h"
#include "still.h"
#include "utility/initializer.h"
#include "world/tile.h"

namespace ql
{
	class game;

	//! Draws the elements of the world that the player can see.
	class world_renderer : public sdl::renderable, effect_const_visitor
	{
	public:
		//! @param world_view The initial world view to render.
		world_renderer(world_view const& world_view) : _world_view{&world_view}, _terrain_render_is_current{false} {}

		//! Updates the world renderer's world view.
		//! @param world_view The new world view to render.
		//! @param effects Any newly perceived effects to render.
		void update_view(world_view const& world_view, std::vector<sptr<effect>> effects);

		//! Updates animations. To be called once per frame.
		void update();

		//! Draws the visible terrain.
		void draw_terrain();

		//! @todo Combine being and object drawing so proper z-buffering is possible.

		//! Draws the visible entities in the world.
		void draw_entities();

		//! Draws visualizations of any active effects in the world.
		void draw_effects();

		//! Causes the tiles for which @p predicate returns true to be highlighted.
		void set_highlight_predicate(std::function<bool(region_tile::point)> predicate);

		//! Clears the current highlight predicate so that no tiles are highlighted.
		void clear_highlight_predicate();

		////////////////////////////
		// Effect Visitor Methods //
		////////////////////////////

		void visit(eagle_eye_effect const&) final;
		void visit(injury_effect const&) final;
		void visit(lightning_bolt_effect const&) final;
	private:
		using entity_id_var_t = std::variant<id<being>, id<object>>;
		using entity_cref_var_t = std::variant<cref<being>, cref<object>>;
		using entity_cptr_var_t = std::variant<being const*, object const*>;

		static std::optional<still> _unknown_entity_animation;

		friend class initializer<world_renderer>;
		static initializer<world_renderer> _initializer;
		static void initialize();

		static entity_id_var_t get_entity_id_var(entity_cref_var_t entity);
		static entity_cref_var_t get_entity_cref_var(entity_id_var_t id);
		static entity const* get_entity_cptr(entity_id_var_t id); 

		world_view const* _world_view;

		std::unordered_map<tile_subtype, uptr<sdl::texture>> _tile_textures;
		std::unordered_map<entity_id_var_t, uptr<animation>> _entity_animation_map;
		uptr<sdl::texture> _terrain_texture;
		units::game_space::box _terrain_bounds;
		bool _terrain_render_is_current;

		std::vector<std::pair<uptr<animation>, units::game_space::point>> _animations;
		
		std::optional<std::function<bool(region_tile::point)>> _highlight_predicate;

		void refresh() final;

		sdl::texture& cache_tile_texture(tile const& tile);
		animation& cache_animation(entity_id_var_t entity);
		animation& get_animation(entity_id_var_t entity);
		void render_terrain();
	};
}
