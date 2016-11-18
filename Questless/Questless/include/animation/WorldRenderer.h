/**
* @file    WorldRenderer.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the WorldRenderer class, which draws the elements of the world that the player can see.
*/

#ifndef WORLD_RENDERER_H
#define WORLD_RENDERER_H

#include <unordered_map>
#include <memory>

#include "Camera.h"
#include "AnimationCollection.h"
#include "entities/Entity.h"
#include "entities/beings/WorldView.h"
#include "sdl-wrappers/Renderer.h"
#include "world/Tile.h"
#include "sdl-wrappers/Renderable.h"
#include "effects/Effect.h"
#include "animation/particles/Particle.h"
#include "utility/Initializer.h"

namespace questless
{
	class Game;

	class WorldRenderer : public sdl::Renderable, EffectVisitor
	{
	public:
		/// @param world_view The initial world view to render.
		WorldRenderer(const WorldView& world_view) : _world_view{&world_view}, _terrain_render_is_current{false} {}

		/// Updates the world renderer's world view.
		/// @param world_view The new world view to render.
		/// @param effects Any newly perceived effects to render.
		void update_view(const WorldView& world_view, std::vector<Effect::ptr> effects);

		/// Updates animations. To be called once per frame.
		void update();

		/// Draws the visible terrain.
		/// @param camera The camera with which to draw the terrain.
		void draw_terrain(const Camera& camera);

		/// Draws the visible beings in the world.
		/// @param game The game object.
		/// @param camera The camera with which to draw the beings.
		void draw_beings(const Game& game, const Camera& camera);

		/// Draws the visible objects in the world.
		/// @param game The game object.
		/// @param camera The camera with which to draw the objects.
		void draw_objects(const Game& game, const Camera& camera);

		/// Draws visualizations of any active effects in the world.
		/// @param game The game object.
		/// @param camera The camera with which to draw the objects.
		void draw_effects(const Game& game, const Camera& camera);

		// Effect visitor methods.

		void visit(const LightningBoltEffect&) override;
	private:
		friend class Initializer<WorldRenderer>;
		static Initializer<WorldRenderer> _initializer;
		static void initialize();

		static sdl::Handle<sdl::Sound> _lightning_bolt_sound_handle;

		const WorldView* _world_view;

		std::unordered_map<Tile::TileClass, sdl::Texture::ptr> _tile_textures;
		std::unordered_map<BeingId, std::unique_ptr<AnimationCollection>> _being_animations;
		std::unordered_map<ObjectId, std::unique_ptr<AnimationCollection>> _object_animations;
		sdl::Texture::ptr _terrain_texture;
		sdl::Rect _terrain_bounds;
		bool _terrain_render_is_current;

		std::vector<Particle::ptr> _particles;

		void refresh() override;

		sdl::Texture& cache_tile_texture(const Tile& tile);
		AnimationCollection& cache_being_animation(const Being& being);
		AnimationCollection& cache_object_animation(const Object& object);
		void render_terrain();
	};
}

#endif
