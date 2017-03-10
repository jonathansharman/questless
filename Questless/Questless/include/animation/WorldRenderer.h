/**
* @file    WorldRenderer.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the WorldRenderer class, which draws the elements of the world that the player can see.
*/

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

	class WorldRenderer : public sdl::Renderable, EffectVisitor
	{
	public:
		/// @param world_view The initial world view to render.
		WorldRenderer(WorldView const& world_view) : _world_view{&world_view}, _terrain_render_is_current{false} {}

		/// Updates the world renderer's world view.
		/// @param world_view The new world view to render.
		/// @param effects Any newly perceived effects to render.
		void update_view(WorldView const& world_view, std::vector<Effect::ptr> effects);

		/// Updates animations. To be called once per frame.
		void update();

		/// Draws the visible terrain.
		/// @param camera The camera with which to draw the terrain.
		void draw_terrain(Camera const& camera);

		/// Draws the visible beings in the world.
		/// @param game The game object.
		/// @param camera The camera with which to draw the beings.
		////
		void draw_beings(Game const& game, Camera const& camera);

		/// Draws the visible objects in the world.
		/// @param game The game object.
		/// @param camera The camera with which to draw the objects.
		////
		void draw_objects(Game const& game, Camera const& camera);

		/// Draws visualizations of any active effects in the world.
		/// @param game The game object.
		/// @param camera The camera with which to draw the objects.
		////
		void draw_effects(Game const& game, Camera const& camera);

		// Effect visitor methods.

		void visit(LightningBoltEffect const&) override;
		void visit(InjuryEffect const&) override;
	private:
		WorldView const* _world_view;

		std::unordered_map<Tile::TileClass, sdl::Texture::ptr> _tile_textures;
		std::unordered_map<Id<Being>, std::unique_ptr<AnimationSet>> _being_animation_sets;
		std::unordered_map<Id<Object>, std::unique_ptr<AnimationSet>> _object_animations;
		sdl::Texture::ptr _terrain_texture;
		units::GameRect _terrain_bounds;
		bool _terrain_render_is_current;

		std::vector<Particle::ptr> _particles;

		void refresh() override;

		sdl::Texture& cache_tile_texture(Tile const& tile);
		AnimationSet& cache_being_animation(Being const& being);
		AnimationSet& cache_object_animation(Object const& object);
		void render_terrain();
	};
}
