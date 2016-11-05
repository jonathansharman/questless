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
#include "animation/particles/YellowMagic.h"

using namespace sdl;

namespace questless
{
	void WorldRenderer::update_view(const WorldView& world_view, std::vector<Effect::ptr> effects)
	{
		_world_view = &world_view;
		_terrain_render_is_current = false;

		for (const auto& effect : effects) {
			effect->accept(*this);
		}

		/// @todo Should the tile texture and entity animation caches ever be cleaned out? If so, when and how?
	}

	void WorldRenderer::update()
	{
		// Update cached being and object animations.

		for (const auto& id_and_animation : _being_animations) {
			if (id_and_animation.second != nullptr) {
				id_and_animation.second->update();
			}
		}
		for (const auto& id_and_animation : _object_animations) {
			if (id_and_animation.second != nullptr) {
				id_and_animation.second->update();
			}
		}

		// Update effect sounds and animations.

		for (size_t i = 0; i < _particles.size();) {
			_particles[i]->update();
			if (_particles[i]->dead()) {
				_particles.erase(_particles.begin() + i);
			} else {
				++i;
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
				auto it = _being_animations.find(being_view.id);
				// If it's there, use it. Otherwise, create the animation and cache it.
				AnimationCollection& being_animation = it != _being_animations.end()
					? *it->second
					: cache_being_animation(*being);

				being_animation.draw(Layout::dflt().to_world(being->coords()).to_point(), camera);
			} else {
				// Remove the being from the animation cache if it doesn't exist anymore.
				_being_animations.erase(being_view.id);
			}
		}
	}

	void WorldRenderer::draw_objects(const Game& game, const Camera& camera)
	{
		for (const auto& object_view : _world_view->object_views()) {
			// Attempt to load the object.
			if (const Object* object = game.object(object_view.id)) {
				// Search for the object's animation in the cache.
				auto it = _object_animations.find(object_view.id);
				// If it's there, use it. Otherwise, create the animation and cache it.
				AnimationCollection& being_animation = it != _object_animations.end()
					? *it->second
					: cache_object_animation(*object);

				being_animation.draw(Layout::dflt().to_world(object->coords()).to_point(), camera);
			} else {
				// Remove the object from the animation cache if it doesn't exist anymore.
				_object_animations.erase(object_view.id);
			}
		}
	}

	void WorldRenderer::draw_effects(const Game& game, const Camera& camera)
	{
		for (auto& particle : _particles) {
			particle->draw(camera);
		}
	}

	void WorldRenderer::visit(const LightningBoltEffect& e)
	{
		RegionTileCoords origin = e.origin();
		for (int i = 0; i < 15; ++i) {
			_particles.emplace_back(YellowMagic::make(Layout::dflt().to_world(origin)));
		}
		//if (_input.down(SDLK_t))
		//	for (int i = 0; i < 3; ++i) _particles.emplace_back(make_unique<WhiteMagic>(_camera->pt_hovered()));
		//if (_input.down(SDLK_y))
		//	for (int i = 0; i < 3; ++i) _particles.emplace_back(make_unique<BlackMagic>(_camera->pt_hovered()));
		//if (_input.down(SDLK_u))
		//	for (int i = 0; i < 3; ++i) _particles.emplace_back(make_unique<GreenMagic>(_camera->pt_hovered()));
		//if (_input.down(SDLK_i))
		//	for (int i = 0; i < 3; ++i) _particles.emplace_back(make_unique<RedMagic>(_camera->pt_hovered()));
		//if (_input.down(SDLK_o))
		//	for (int i = 0; i < 3; ++i) _particles.emplace_back(make_unique<BlueMagic>(_camera->pt_hovered()));
		//if (_input.down(SDLK_p))
		//	for (int i = 0; i < 3; ++i) _particles.emplace_back(make_unique<YellowMagic>(_camera->pt_hovered()));
	}

	void WorldRenderer::refresh()
	{
		_tile_textures.clear();
		_terrain_render_is_current = false;
	}

	Texture& WorldRenderer::cache_tile_texture(const Tile& tile)
	{
		TileTexturer tile_texturer;
		tile.accept(tile_texturer);
		_tile_textures[tile.tile_class()] = tile_texturer.texture();
		return *_tile_textures[tile.tile_class()];
	};

	AnimationCollection& WorldRenderer::cache_being_animation(const Being& being)
	{
		EntityAnimator entity_animator;
		being.accept(entity_animator);
		_being_animations[being.id()] = entity_animator.animation();
		return *_being_animations[being.id()];
	};

	AnimationCollection& WorldRenderer::cache_object_animation(const Object& object)
	{
		EntityAnimator entity_animator;
		object.accept(entity_animator);
		_object_animations[object.id()] = entity_animator.animation();
		return *_object_animations[object.id()];
	};

	void WorldRenderer::render_terrain()
	{
		boost::optional<Rect> opt_bounds = _world_view->bounds();
		if (!opt_bounds) {
			_terrain_texture = nullptr;
			return;
		}
		_terrain_bounds = *opt_bounds;

		_terrain_texture = std::make_unique<Texture>
			( sdl::renderer()
			, SDL_BLENDMODE_BLEND
			, _terrain_bounds.w
			, _terrain_bounds.h
			, true
			);
		_terrain_texture->as_target([&] {
			renderer().clear(Color::clear());
			for (const auto& section_view : _world_view->section_views()) {
				auto opt_section = _world_view->region().section(section_view.coords);
				if (opt_section) {
					const Section& section = *opt_section;
					for (int r = -section_radius; r <= section_radius; ++r) {
						for (int q = -section_radius; q <= section_radius; ++q) {
							SectionTileCoords section_tile_coords{q, r};
							SectionTileIndex tile_index = Section::tile_index(section_tile_coords);
							double tile_visibility = section_view.tile_visibilities[tile_index.i][tile_index.j];
							if (tile_visibility > 0) {
								RegionTileCoords region_tile_coords = section.region_tile_coords(section_tile_coords);
								Point coords_in_world = Layout::dflt().to_world(region_tile_coords).to_point();
								uint8_t luminance = static_cast<uint8_t>(255 * std::min(tile_visibility / 100.0, 1.0));

								// Get the current tile.
								const Tile& tile = section.tile(section_tile_coords);
								// Search for its texture in the cache.
								auto it = _tile_textures.find(tile.tile_class());
								// If it's there, use it. Otherwise, create the texture and cache it.
								Texture& tile_texture = it != _tile_textures.end() ? *it->second : cache_tile_texture(tile);

								tile_texture.draw_transformed
									( coords_in_world - Vector::to(_terrain_bounds.position())
									, boost::none // Origin
									, 1.0 // H-scale
									, 1.0 // V-scale
									, false // H-flip
									, false // V-flip
									, Color{luminance, luminance, luminance});
							}
						}
					}
				}
			}
		});
	}
}
