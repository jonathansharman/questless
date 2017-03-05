/**
* @file    WorldRenderer.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the WorldRenderer class.
*/

#include "animation/WorldRenderer.h"

#include "Game.h"
#include "animation/EntityAnimator.h"
#include "animation/TileTexturer.h"
#include "animation/particles/YellowMagicParticle.h"
#include "animation/particles/BloodParticle.h"
#include "animation/particles/TextParticle.h"
#include "world/Region.h"
#include "utility/utility.h"

using std::make_unique;

using namespace sdl;
using namespace units;

namespace questless
{
	void WorldRenderer::update_view(WorldView const& world_view, std::vector<Effect::ptr> effects)
	{
		_world_view = &world_view;
		_terrain_render_is_current = false;

		for (auto const& effect : effects) {
			effect->accept(*this);
		}

		/// @todo Should the tile texture and entity animation caches ever be cleaned out? If so, when and how? Consider tracking how long since a texture was last used and purging it after a set time.
	}

	void WorldRenderer::update()
	{
		// Update cached being and object animations.

		for (auto const& id_and_animation : _being_animation_sets) {
			if (id_and_animation.second != nullptr) {
				id_and_animation.second->update();
			}
		}
		for (auto const& id_and_animation : _object_animations) {
			if (id_and_animation.second != nullptr) {
				id_and_animation.second->update();
			}
		}

		// Update effect sounds and animations.

		for (size_t i = 0; i < _particles.size();) {
			_particles[i]->update();
			if (_particles[i]->expired()) {
				_particles.erase(_particles.begin() + i);
			} else {
				++i;
			}
		}
	}

	void WorldRenderer::draw_terrain(Camera const& camera)
	{
		if (!_terrain_render_is_current) {
			render_terrain();
			_terrain_render_is_current = true;
		}
		if (_terrain_texture) {
			camera.draw(*_terrain_texture, GamePoint{_terrain_bounds.position()}, Origin{GamePoint{0, 0}});
		}
	}

	void WorldRenderer::draw_beings(Game const& game, Camera const& camera)
	{
		for (auto const& being_view : _world_view->being_views()) {
			// Attempt to load the being.
			if (Being const* being = game.being(being_view.id)) {
				// Search for the being's animation in the cache.
				auto it = _being_animation_sets.find(being_view.id);
				// If it's there, use it. Otherwise, create the animation and cache it.
				AnimationSet& being_animation = it != _being_animation_sets.end()
					? *it->second
					: cache_being_animation(*being);

				// Draw heading.
				GamePoint start = Layout::dflt().to_world(being->coords);
				GamePoint end = Layout::dflt().to_world(being->coords.neighbor(being->direction));
				camera.draw_lines({start, end}, Color::magenta());

				uint8_t luminance;
				switch (being_view.perception) {
					case WorldView::BeingView::Perception::low:
						luminance = 128;
						break;
					case WorldView::BeingView::Perception::medium:
						luminance = 192;
						break;
					case WorldView::BeingView::Perception::high:
					case WorldView::BeingView::Perception::full:
						luminance = 255;
						break;
					default:
						throw std::logic_error{"Invalid perception level."};
				}
				being_animation.draw(Layout::dflt().to_world(being->coords), camera, Color{luminance, luminance, luminance});
			} else {
				// Remove the being from the animation cache if it doesn't exist anymore.
				_being_animation_sets.erase(being_view.id);
			}
		}
	}

	void WorldRenderer::draw_objects(Game const& game, Camera const& camera)
	{
		for (auto const& object_view : _world_view->object_views()) {
			// Attempt to load the object.
			if (Object const* object = game.object(object_view.id)) {
				// Search for the object's animation in the cache.
				auto it = _object_animations.find(object_view.id);
				// If it's there, use it. Otherwise, create the animation and cache it.
				AnimationSet& object_animation = it != _object_animations.end()
					? *it->second
					: cache_object_animation(*object);

				uint8_t luminance;
				switch (object_view.perception) {
					case WorldView::ObjectView::Perception::low:
						luminance = 128;
						break;
					case WorldView::ObjectView::Perception::medium:
						luminance = 192;
						break;
					case WorldView::ObjectView::Perception::high:
					case WorldView::ObjectView::Perception::full:
						luminance = 255;
						break;
					default:
						throw std::logic_error{"Invalid perception level."};
				}
				object_animation.draw(Layout::dflt().to_world(object->coords), camera, Color{luminance, luminance, luminance});
			} else {
				// Remove the object from the animation cache if it doesn't exist anymore.
				_object_animations.erase(object_view.id);
			}
		}
	}

	void WorldRenderer::draw_effects(Game const&, Camera const& camera)
	{
		for (auto& particle : _particles) {
			particle->draw(camera);
		}
	}

	void WorldRenderer::refresh()
	{
		_tile_textures.clear();
		_terrain_render_is_current = false;
	}

	Texture& WorldRenderer::cache_tile_texture(Tile const& tile)
	{
		TileTexturer tile_texturer;
		tile.accept(tile_texturer);
		_tile_textures[tile.tile_class()] = tile_texturer.texture();
		return *_tile_textures[tile.tile_class()];
	};

	AnimationSet& WorldRenderer::cache_being_animation(Being const& being)
	{
		EntityAnimator entity_animator;
		being.accept(entity_animator);
		_being_animation_sets[being.id()] = entity_animator.animation_set();
		return *_being_animation_sets[being.id()];
	};

	AnimationSet& WorldRenderer::cache_object_animation(Object const& object)
	{
		EntityAnimator entity_animator;
		object.accept(entity_animator);
		_object_animations[object.id()] = entity_animator.animation_set();
		return *_object_animations[object.id()];
	};

	void WorldRenderer::render_terrain()
	{
		boost::optional<GameRect> opt_bounds = _world_view->bounds();
		if (!opt_bounds) {
			_terrain_texture = nullptr;
			return;
		}
		_terrain_bounds = *opt_bounds;

		_terrain_texture = make_unique<Texture>(lround(_terrain_bounds.w), lround(_terrain_bounds.h));
		_terrain_texture->as_target([&] {
			renderer().clear(Color::clear());
			for (auto const& section_view : _world_view->section_views()) {
				auto opt_section = _world_view->region().section(section_view.coords);
				if (opt_section) {
					Section const& section = *opt_section;
					for (int r = -Section::radius; r <= Section::radius; ++r) {
						for (int q = -Section::radius; q <= Section::radius; ++q) {
							SectionTileCoords section_tile_coords{q, r};
							SectionTileIndex tile_index = Section::tile_index(section_tile_coords);
							double tile_visibility = section_view.tile_visibilities[tile_index.i][tile_index.j];
							if (tile_visibility > 0) {
								RegionTileCoords const region_tile_coords = section.region_tile_coords(section_tile_coords);
								GamePoint const tile_game_point = Layout::dflt().to_world(region_tile_coords);
								GamePoint const terrain_game_point = _terrain_bounds.position();
								ScreenPoint const tile_screen_point
									{ lround(tile_game_point.x - terrain_game_point.x)
									, lround(terrain_game_point.y - tile_game_point.y + _terrain_bounds.h - 1)
									};

								uint8_t const luminance = static_cast<uint8_t>(255 * std::min(tile_visibility / 100.0, 1.0));

								// Get the current tile.
								Tile const& tile = section.tile(section_tile_coords);
								// Search for its texture in the cache.
								auto it = _tile_textures.find(tile.tile_class());
								// If it's there, use it. Otherwise, create the texture and cache it.
								Texture& tile_texture = it != _tile_textures.end() ? *it->second : cache_tile_texture(tile);

								tile_texture.draw_transformed
									( tile_screen_point
									, boost::none // origin
									, Color{luminance, luminance, luminance}
									);
							}
						}
					}
				}
			}
		});
	}

	// Effect Visitor Functions

	void WorldRenderer::visit(LightningBoltEffect const& e)
	{
		static auto lightning_bolt_sound_handle = sound_manager().add("resources/sounds/spells/lightning-bolt.wav");

		GamePoint position = Layout::dflt().to_world(e.origin());
		for (int i = 0; i < 15; ++i) {
			_particles.emplace_back(make_unique<YellowMagicParticle>(position));
		}
		sound_manager()[lightning_bolt_sound_handle].play();
	}

	void WorldRenderer::visit(InjuryEffect const& e)
	{
		static auto hit_sound_handle = sound_manager().add("resources/sounds/weapons/hit.wav");

		GamePoint position = Layout::dflt().to_world(e.origin());

		Damage const& d = e.damage();
		if (d.slash() > 0.0 || d.pierce() > 0.0 || d.bludgeon() > 0.0) {
			for (int i = 0; i < 20; ++i) {
				_particles.emplace_back(make_unique<BloodParticle>(position));
			}
		}

		if (d.slash() > 0.0) {
			_particles.emplace_back(make_unique<TextParticle>(position, std::to_string(lround(e.damage().slash())), Color::white()));
		}
		if (d.pierce() > 0.0) {
			_particles.emplace_back(make_unique<TextParticle>(position, std::to_string(lround(e.damage().pierce())), Color::white()));
		}
		if (d.bludgeon() > 0.0) {
			_particles.emplace_back(make_unique<TextParticle>(position, std::to_string(lround(e.damage().bludgeon())), Color::white()));
			sound_manager()[hit_sound_handle].play();
		}
		if (d.burn() > 0.0) {
			_particles.emplace_back(make_unique<TextParticle>(position, std::to_string(lround(e.damage().burn())), Color::orange()));
		}
		if (d.freeze() > 0.0) {
			_particles.emplace_back(make_unique<TextParticle>(position, std::to_string(lround(e.damage().freeze())), Color::cyan()));
		}
		if (d.blight() > 0.0) {
			_particles.emplace_back(make_unique<TextParticle>(position, std::to_string(lround(e.damage().blight())), Color::black()));
		}
	}
}
