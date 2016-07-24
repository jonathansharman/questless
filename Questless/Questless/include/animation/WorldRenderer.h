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

namespace questless
{
	class Game;

	class WorldRenderer : public sdl::Renderable
	{
	public:
		/// @param world_view The world view to render.
		WorldRenderer(const WorldView& world_view) : _world_view{&world_view}, _terrain_render_is_current{false} {}

		/// Resets the world view to a new view.
		/// @param world_view The new world view to render.
		void reset_view(const WorldView& world_view);

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
	private:
		const WorldView* _world_view;

		std::unordered_map<Tile::TileClass, sdl::Texture::ptr> _tile_textures;
		std::unordered_map<Entity::id_t, std::unique_ptr<AnimationCollection>> _entity_animations;
		sdl::Texture::ptr _terrain_texture;
		sdl::Rect _terrain_bounds;
		bool _terrain_render_is_current;

		void refresh() override;

		sdl::Texture& create_and_cache_tile_texture(const Tile& tile);
		AnimationCollection& create_and_cache_entity_animation(const Entity& entity);
		void render_terrain();
	};
}

#endif
