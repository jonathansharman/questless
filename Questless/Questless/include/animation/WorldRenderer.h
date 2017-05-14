//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>
#include <unordered_map>
#include <variant>

#include "Camera.h"
#include "effects/Effect.h"
#include "entities/beings/WorldView.h"
#include "entities/Entity.h"
#include "sdl/Renderable.h"
#include "sdl/Renderer.h"
#include "Still.h"
#include "utility/Initializer.h"
#include "world/Tile.h"

namespace questless
{
	class Game;

	//! Draws the elements of the world that the player can see.
	class WorldRenderer : public sdl::Renderable, EffectConstVisitor
	{
	public:
		//! @param world_view The initial world view to render.
		WorldRenderer(WorldView const& world_view) : _world_view{&world_view}, _terrain_render_is_current{false} {}

		//! Updates the world renderer's world view.
		//! @param world_view The new world view to render.
		//! @param effects Any newly perceived effects to render.
		void update_view(WorldView const& world_view, std::vector<sptr<Effect>> effects);

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
		void set_highlight_predicate(std::function<bool(RegionTile::Point)> predicate);

		//! Clears the current highlight predicate so that no tiles are highlighted.
		void clear_highlight_predicate();

		////////////////////////////
		// Effect Visitor Methods //
		////////////////////////////

		void visit(EagleEyeEffect const&) final;
		void visit(InjuryEffect const&) final;
		void visit(LightningBoltEffect const&) final;
	private:
		using entity_id_var_t = std::variant<Id<Being>, Id<Object>>;
		using entity_cref_var_t = std::variant<cref<Being>, cref<Object>>;
		using entity_cptr_var_t = std::variant<Being const*, Object const*>;

		static std::optional<Still> _unknown_entity_animation;

		friend class Initializer<WorldRenderer>;
		static Initializer<WorldRenderer> _initializer;
		static void initialize();

		static entity_id_var_t get_entity_id_var(entity_cref_var_t entity);
		static entity_cref_var_t get_entity_cref_var(entity_id_var_t id);
		static Entity const* get_entity_cptr(entity_id_var_t id); 

		WorldView const* _world_view;

		std::unordered_map<Tile::TileClass, uptr<sdl::Texture>> _tile_textures;
		std::unordered_map<entity_id_var_t, uptr<Animation>> _entity_animation_map;
		uptr<sdl::Texture> _terrain_texture;
		units::GameRect _terrain_bounds;
		bool _terrain_render_is_current;

		std::vector<std::pair<uptr<Animation>, units::GamePoint>> _animations;
		
		std::optional<std::function<bool(RegionTile::Point)>> _highlight_predicate;

		void refresh() final;

		sdl::Texture& cache_tile_texture(Tile const& tile);
		Animation& cache_animation(entity_id_var_t entity);
		Animation& get_animation(entity_id_var_t entity);
		void render_terrain();
	};
}
