//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <unordered_map>
#include <memory>

#include "Camera.h"
#include "effects/Effect.h"
#include "entities/beings/WorldView.h"
#include "entities/Entity.h"
#include "particles/Particle.h"
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

		//! Draws the visible beings in the world.
		void draw_beings();

		//! Draws the visible objects in the world.
		void draw_objects();

		//! Draws visualizations of any active effects in the world.
		void draw_effects();

		////////////////////////////
		// Effect Visitor Methods //
		////////////////////////////

		void visit(EagleEyeEffect const&) final;
		void visit(InjuryEffect const&) final;
		void visit(LightningBoltEffect const&) final;
	private:
		static std::optional<Still> _unknown_entity_animation;

		friend class Initializer<WorldRenderer>;
		static Initializer<WorldRenderer> _initializer;
		static void initialize();

		WorldView const* _world_view;

		std::unordered_map<Tile::TileClass, uptr<sdl::Texture>> _tile_textures;
		std::unordered_map<Id<Being>, uptr<Animation>> _being_animation_map;
		std::unordered_map<Id<Object>, uptr<Animation>> _object_animation_map; //! @todo It would be nice to combine the being and object animation maps since the logic is idenitical. Could add an Entity::id field.
		uptr<sdl::Texture> _terrain_texture;
		units::GameRect _terrain_bounds;
		bool _terrain_render_is_current;

		std::vector<uptr<Particle>> _particles;

		void refresh() final;

		sdl::Texture& cache_tile_texture(Tile const& tile);
		Animation& cache_animation(Being const& being);
		Animation& cache_animation(Object const& object);
		Animation& get_animation(Being const& being);
		Animation& get_animation(Object const& being);
		void render_terrain();
	};
}
