//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <unordered_map>
#include <variant>

#include "effects/effect_visitor.hpp"
#include "sprite_animation.hpp"
#include "utility/id.hpp"
#include "utility/initializer.hpp"
#include "utility/reference.hpp"
#include "world/coordinates.hpp"
#include "world/tile_visitor.hpp"

namespace ql {
	class being;
	class effect;
	class entity;
	class game;
	class object;
	class still;
	class tile;
	class world_view;

	//! Draws the elements of the world that the player can see.
	class world_renderer : public effect_const_visitor {
	public:
		//! @param world_view The initial world view to render.
		world_renderer(world_view const& world_view);

		//! Updates the world renderer's world view.
		//! @param world_view The new world view to render.
		//! @param effects Any newly perceived effects to render.
		void update_view(world_view const& world_view, std::vector<sptr<effect>> effects);

		//! Updates animations. To be called once per frame.
		void update();

		//! Draws the visible terrain.
		void draw_terrain(camera const& camera);

		//! @todo Combine being and object drawing so proper z-buffering is possible.

		//! Draws the visible entities in the world.
		void draw_entities(camera const& camera);

		//! Draws visualizations of any active effects in the world.
		void draw_effects(camera const& camera);

		//! Causes the tiles for which @p predicate returns true to be highlighted.
		void set_highlight_predicate(std::function<bool(region_tile::point)> predicate);

		//! Clears the current highlight predicate so that no tiles are highlighted.
		void clear_highlight_predicate();

		////////////////////////////
		// Effect Visitor Methods //
		////////////////////////////

		void visit(arrow_attack_effect const&) final;
		void visit(eagle_eye_effect const&) final;
		void visit(injury_effect const&) final;
		void visit(lightning_bolt_effect const&) final;
	private:
		using entity_id_var_t = std::variant<id<being>, id<object>>;
		using entity_cref_var_t = std::variant<cref<being>, cref<object>>;
		using entity_cptr_var_t = std::variant<being const*, object const*>;

		static uptr<still> _unknown_entity_animation;

		friend class initializer<world_renderer>;
		static initializer<world_renderer> _initializer;
		static void initialize();

		static entity_id_var_t get_entity_id_var(entity_cref_var_t entity);
		static entity_cref_var_t get_entity_cref_var(entity_id_var_t id);
		static entity const* get_entity_cptr(entity_id_var_t id); 

		world_view const* _world_view;

		sprite_animation _tile_selector_animation;
		std::unordered_map<tile_subtype, uptr<sdl::texture>> _tile_textures;
		std::unordered_map<entity_id_var_t, uptr<animation>> _entity_animation_map;
		uptr<sdl::texture> _terrain_texture;
		units::world_space::box _terrain_bounds{};
		bool _terrain_render_is_current;

		std::vector<std::pair<uptr<animation>, units::world_space::point>> _animations;
		
		std::optional<std::function<bool(region_tile::point)>> _highlight_predicate;

		sdl::texture& cache_tile_texture(tile const& tile);
		animation& cache_animation(entity_id_var_t entity);
		animation& get_animation(entity_id_var_t entity);
		void render_terrain();
	};
}
