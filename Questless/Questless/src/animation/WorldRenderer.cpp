/**
* @file    WorldRenderer.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the WorldRenderer class.
*/

#include "animation/WorldRenderer.h"
#include "animation/EntityAnimator.h"
#include "world/Region.h"
#include "sdl-wrappers/Renderable.h"
#include "animation/TileTexturer.h"

namespace questless
{
	void WorldRenderer::update()
	{
		for (const auto& id_and_animation : _entity_animations) {
			if (id_and_animation.second != nullptr) {
				id_and_animation.second->update();
			}
		}
	}

	Texture& WorldRenderer::create_and_cache_tile_texture(const Tile& tile) {
		TileTexturer tile_texturer;
		tile.accept(tile_texturer);
		_tile_textures[tile.type()] = tile_texturer.texture();
		return *_tile_textures[tile.type()];
	};

	void WorldRenderer::draw_terrain(const WorldView& world_view, const Camera& camera)
	{
		for (const auto& section_view : world_view.section_views()) {
			const Section& section = world_view.region().section(section_view.coords);
			for (int r = -section_radius; r <= section_radius; ++r) {
				for (int q = -section_radius; q <= section_radius; ++q) {
					double tile_visibility = section_view.tile_visibilities[r + section_radius][q + section_radius];
					if (tile_visibility >= 0) {
						HexCoords tile_coords_section{q, r};
						HexCoords tile_coords_region = section.coords() * section_diameter + tile_coords_section;
						Point tile_coords_world = Layout::dflt().to_world(tile_coords_region);
						uint8_t luminance = static_cast<uint8_t>(255 * std::min(tile_visibility / 100.0, 1.0));

						// Get the current tile.
						const Tile& tile = section.tile(tile_coords_section);
						// Search for its texture in the cache.
						auto it = _tile_textures.find(tile.type());
						// If it's there, use it. Otherwise, create the texture and cache it.
						Texture& tile_texture = it != _tile_textures.end() ? *it->second : create_and_cache_tile_texture(tile);

						camera.draw(tile_texture, tile_coords_world, nullopt, 1.0, 1.0, false, false, 0.0, Color{luminance, luminance, luminance});
					}
				}
			}
		}
	}

	AnimationCollection& WorldRenderer::create_and_cache_entity_animation(Entity& entity)
	{
		EntityAnimator entity_animator;
		entity.accept(entity_animator);
		_entity_animations[entity.entity_id()] = entity_animator.animation();
		return *_entity_animations[entity.entity_id()];
	};

	void WorldRenderer::draw_beings(const WorldView& world_view, const Camera& camera)
	{
		for (const auto& being_view : world_view.being_views()) {
			// Search for the being's animation in the cache.
			auto it = _entity_animations.find(being_view.being->entity_id());
			// If it's there, use it. Otherwise, create the animation and cache it.
			AnimationCollection& being_animation = it != _entity_animations.end() ? *it->second : create_and_cache_entity_animation(*being_view.being);

			being_animation.draw(Layout::dflt().to_world(being_view.being->coords()), camera);
		}
	}
}