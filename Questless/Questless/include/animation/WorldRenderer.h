//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <unordered_map>
#include <memory>

#include "Camera.h"
#include "AnimationSet.h"
#include "entities/Entity.h"
#include "entities/beings/WorldView.h"
#include "sdl/Renderer.h"
#include "world/Tile.h"
#include "sdl/Renderable.h"
#include "effects/Effect.h"
#include "animation/particles/Particle.h"

namespace questless
{
	class Game;

	//! Draws the elements of the world that the player can see.
	class WorldRenderer : public sdl::Renderable, EffectVisitor
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

		void visit(EagleEyeEffect const&) override;
		void visit(InjuryEffect const&) override;
		void visit(LightningBoltEffect const&) override;
	private:
		WorldView const* _world_view;

		std::unordered_map<Tile::TileClass, uptr<sdl::Texture>> _tile_textures;
		std::unordered_map<Id<Being>, std::unique_ptr<AnimationSet>> _being_animation_sets;
		std::unordered_map<Id<Object>, std::unique_ptr<AnimationSet>> _object_animations;
		uptr<sdl::Texture> _terrain_texture;
		units::GameRect _terrain_bounds;
		bool _terrain_render_is_current;

		std::vector<uptr<Particle>> _particles;

		void refresh() override;

		sdl::Texture& cache_tile_texture(Tile const& tile);
		AnimationSet& cache_being_animation(Being const& being);
		AnimationSet& cache_object_animation(Object const& object);
		void render_terrain();
	};
}
