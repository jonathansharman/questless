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

#include <memory>
using std::shared_ptr;

#include "Camera.h"
#include "AnimationCollection.h"
#include "entities/Entity.h"
#include "entities/beings/WorldView.h"
#include "sdl-wrappers/Renderer.h"
#include "world/Tile.h"
#include "sdl-wrappers/Renderable.h"
using sdl::Renderable;

namespace questless
{
	class WorldRenderer : public Renderable
	{
	public:
		void refresh() override
		{
			_tile_textures.clear();
			_entity_animations.clear();
		}

		void update();
		void draw_terrain(const WorldView& world_view, const Camera& camera);
		void draw_beings(const WorldView& world_view, const Camera& camera);
	private:
		map<Tile::Type, Texture::ptr> _tile_textures;
		map<Entity::id_t, unique_ptr<AnimationCollection>> _entity_animations;

		Texture& create_and_cache_tile_texture(const Tile& tile);
		AnimationCollection& create_and_cache_entity_animation(Entity& entity);
	};
}

#endif