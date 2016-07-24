/**
* @file    WorldRenderer.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the WorldRenderer class.
*/

#include "Game.h"
#include "animation/WorldRenderer.h"
#include "animation/EntityAnimator.h"
#include "world/Region.h"
#include "sdl-wrappers/Renderable.h"
#include "animation/TileTexturer.h"
#include "utility/utility.h"

using namespace sdl;

namespace questless
{
	void WorldRenderer::reset_view(const WorldView& world_view)
	{
		_world_view = &world_view;
		_terrain_render_is_current = false;

		/// @todo Should the tile texture and entity animation caches ever be cleaned out? If so, when and how?
	}

	void WorldRenderer::update()
	{
		for (const auto& id_and_animation : _entity_animations) {
			if (id_and_animation.second != nullptr) {
				id_and_animation.second->update();
			}
		}
	}

	void WorldRenderer::draw_terrain(const Camera& camera)
	{
		if (!_terrain_render_is_current) {
			render_terrain();
			_terrain_render_is_current = true;
		}
		if (_terrain_texture) {
			camera.draw(*_terrain_texture, _terrain_bounds.position(), Point{0, 0});
		}
	}

	void WorldRenderer::draw_beings(const Game& game, const Camera& camera)
	{
		for (const auto& being_view : _world_view->being_views()) {
			// Attempt to load the being.
			if (const Being* being = game.being(being_view.id)) {
				// Search for the being's animation in the cache.
				auto it = _entity_animations.find(being_view.id);
				// If it's there, use it. Otherwise, create the animation and cache it.
				AnimationCollection& being_animation = it != _entity_animations.end()
					? *it->second
					: create_and_cache_entity_animation(being->as_entity());

				being_animation.draw(Layout::dflt().to_world(being->coords().hex), camera);
			} else {
				// Remove the being from the animation cache if it doesn't exist anymore.
				_entity_animations.erase(being_view.id);
			}
		}
	}

	void WorldRenderer::draw_objects(const Game& game, const Camera& camera)
	{
		for (const auto& object_view : _world_view->object_views()) {
			// Attempt to load the object.
			if (const Object* object = game.object(object_view.id)) {
				// Search for the object's animation in the cache.
				auto it = _entity_animations.find(object_view.id);
				// If it's there, use it. Otherwise, create the animation and cache it.
				AnimationCollection& being_animation = it != _entity_animations.end()
					? *it->second
					: create_and_cache_entity_animation(object->as_entity());

				being_animation.draw(Layout::dflt().to_world(object->coords().hex), camera);
			} else {
				// Remove the object from the animation cache if it doesn't exist anymore.
				_entity_animations.erase(object_view.id);
			}
		}
	}

	void WorldRenderer::refresh()
	{
		_tile_textures.clear();
		_terrain_render_is_current = false;
	}

	Texture& WorldRenderer::create_and_cache_tile_texture(const Tile& tile)
	{
		TileTexturer tile_texturer;
		tile.accept(tile_texturer);
		_tile_textures[tile.tile_class()] = tile_texturer.texture();
		return *_tile_textures[tile.tile_class()];
	};

	AnimationCollection& WorldRenderer::create_and_cache_entity_animation(const Entity& entity)
	{
		EntityAnimator entity_animator;
		entity.accept(entity_animator);
		_entity_animations[entity.entity_id()] = entity_animator.animation();
		return *_entity_animations[entity.entity_id()];
	};

	void WorldRenderer::render_terrain()
	{
		optional<Rect> opt_bounds = _world_view->bounds();
		if (!opt_bounds) {
			_terrain_texture = nullptr;
			return;
		}
		_terrain_bounds = *opt_bounds;

		_terrain_texture = make_unique<Texture>
			( sdl::renderer()
			, SDL_BLENDMODE_BLEND
			, _terrain_bounds.w
			, _terrain_bounds.h
			, true
			);
		_terrain_texture->as_target([&] {
			renderer().clear(Color::clear());
			for (const auto& section_view : _world_view->section_views()) {
				const Section& section = _world_view->region().section(section_view.coords);
				for (int r = -section_radius; r <= section_radius; ++r) {
					for (int q = -section_radius; q <= section_radius; ++q) {
						SectionTileCoords section_tile_coords{{q, r}};
						SectionTileIndex tile_index = Section::tile_index(section_tile_coords);
						double tile_visibility = section_view.tile_visibilities[tile_index.i][tile_index.j];
						if (tile_visibility > 0) {
							RegionTileCoords region_tile_coords = section.region_tile_coords(section_tile_coords);
							Point coords_in_world = Layout::dflt().to_world(region_tile_coords.hex);
							uint8_t luminance = static_cast<uint8_t>(255 * std::min(tile_visibility / 100.0, 1.0));

							// Get the current tile.
							const Tile& tile = section.tile(section_tile_coords);
							// Search for its texture in the cache.
							auto it = _tile_textures.find(tile.tile_class());
							// If it's there, use it. Otherwise, create the texture and cache it.
							Texture& tile_texture = it != _tile_textures.end() ? *it->second : create_and_cache_tile_texture(tile);

							tile_texture.draw_transformed(coords_in_world - Vector::to(_terrain_bounds.position()), nullopt, 1.0, 1.0, false, false, Color{luminance, luminance, luminance});
						}
					}
				}
			}
		});
	}
}
