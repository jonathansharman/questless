//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/WorldRenderer.h"

#include "animation/EntityAnimator.h"
#include "animation/particles/BloodParticle.h"
#include "animation/particles/GreenMagicParticle.h"
#include "animation/particles/TextParticle.h"
#include "animation/particles/YellowMagicParticle.h"
#include "animation/TileTexturer.h"
#include "Game.h"
#include "utility/utility.h"
#include "world/Region.h"

using std::make_unique;

using namespace sdl;
using namespace units;

namespace questless
{
	void WorldRenderer::update_view(WorldView const& world_view, std::vector<Effect::uptr> effects)
	{
		_world_view = &world_view;
		_terrain_render_is_current = false;

		for (auto const& effect : effects) {
			effect->accept(*this);
		}

		//! @todo Should the tile texture and entity animation caches ever be cleaned out? If so, when and how? Consider tracking how long since a texture was last used and purging it after a set time.
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

	void WorldRenderer::draw_terrain()
	{
		if (!_terrain_render_is_current) {
			render_terrain();
			_terrain_render_is_current = true;
		}
		if (_terrain_texture) {
			game().camera().draw(*_terrain_texture, GamePoint{_terrain_bounds.position()}, Origin{GamePoint{0, 0}});
		}
	}

	void WorldRenderer::draw_beings()
	{
		for (auto const& being_view : _world_view->being_views()) {
			// Attempt to load the being.
			if (Being const* being = game().beings.get(being_view.id)) {
				// Search for the being's animation in the cache.
				auto it = _being_animation_sets.find(being_view.id);
				// If it's there, use it. Otherwise, create the animation and cache it.
				AnimationSet& being_animation = it != _being_animation_sets.end()
					? *it->second
					: cache_being_animation(*being);

				// Draw heading.
				GamePoint start = Layout::dflt().to_world(being->coords);
				GamePoint end = Layout::dflt().to_world(being->coords.neighbor(being->direction));
				game().camera().draw_lines({start, end}, Color::magenta());

				uint8_t luminance;
				switch (being_view.perception) {
					case WorldView::PerceptionLevel::low:
						luminance = 128;
						break;
					case WorldView::PerceptionLevel::medium:
						luminance = 192;
						break;
					case WorldView::PerceptionLevel::high:
					case WorldView::PerceptionLevel::full:
						luminance = 255;
						break;
					default:
						throw std::logic_error{"Invalid perception level."};
				}
				being_animation.draw(Layout::dflt().to_world(being->coords), game().camera(), Color{luminance, luminance, luminance});
			} else {
				// Remove the being from the animation cache if it doesn't exist anymore.
				_being_animation_sets.erase(being_view.id);
			}
		}
	}

	void WorldRenderer::draw_objects()
	{
		for (auto const& object_view : _world_view->object_views()) {
			// Attempt to load the object.
			if (Object const* object = game().objects.get(object_view.id)) {
				// Search for the object's animation in the cache.
				auto it = _object_animations.find(object_view.id);
				// If it's there, use it. Otherwise, create the animation and cache it.
				AnimationSet& object_animation = it != _object_animations.end()
					? *it->second
					: cache_object_animation(*object);

				uint8_t luminance;
				switch (object_view.perception) {
					case WorldView::PerceptionLevel::low:
						luminance = 128;
						break;
					case WorldView::PerceptionLevel::medium:
						luminance = 192;
						break;
					case WorldView::PerceptionLevel::high:
					case WorldView::PerceptionLevel::full:
						luminance = 255;
						break;
					default:
						throw std::logic_error{"Invalid perception level."};
				}
				object_animation.draw(Layout::dflt().to_world(object->coords), game().camera(), Color{luminance, luminance, luminance});
			} else {
				// Remove the object from the animation cache if it doesn't exist anymore.
				_object_animations.erase(object_view.id);
			}
		}
	}

	void WorldRenderer::draw_effects()
	{
		for (auto& particle : _particles) {
			particle->draw(game().camera());
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
		_being_animation_sets[being.id] = entity_animator.animation_set();
		return *_being_animation_sets[being.id];
	};

	AnimationSet& WorldRenderer::cache_object_animation(Object const& object)
	{
		EntityAnimator entity_animator;
		object.accept(entity_animator);
		_object_animations[object.id] = entity_animator.animation_set();
		return *_object_animations[object.id];
	};

	void WorldRenderer::render_terrain()
	{
		std::optional<GameRect> opt_bounds = _world_view->bounds();
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
					for (int q = 0; q < Section::diameter; ++q) {
						for (int r = 0; r < Section::diameter; ++r) {
							SectionTileCoords section_tile_coords{q, r};
							double tile_visibility = section_view.tile_visibilities[section_tile_coords.q][section_tile_coords.r];
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
									, std::nullopt // origin
									, Color{luminance, luminance, luminance}
									);
							}
						}
					}
				}
			}
		});
	}

	//////////////////////////////
	// Effect Visitor Functions //
	//////////////////////////////

	void WorldRenderer::visit(EagleEyeEffect const& e)
	{
		static auto eagle_eye_sound_handle = sound_manager().add("resources/sounds/spells/eagle-eye.wav");

		GamePoint position = Layout::dflt().to_world(e.origin());
		for (int i = 0; i < 50; ++i) {
			_particles.emplace_back(make_unique<GreenMagicParticle>(position));
		}
		sound_manager()[eagle_eye_sound_handle].play();
	}

	void WorldRenderer::visit(InjuryEffect const& e)
	{
		static auto pierce_sound_handle = sound_manager().add("resources/sounds/weapons/pierce.wav");
		static auto hit_sound_handle = sound_manager().add("resources/sounds/weapons/hit.wav");

		Being const* target = game().beings.get(e.target_id);
		double const target_vitality = target ? target->stats.vitality.get() : 100.0; // Assume vitality = 100 if being no longer exists to check.
		//! @todo Pass along the vitality in the event object if it's needed here.

		GamePoint position = Layout::dflt().to_world(e.origin());

		Damage const& damage = e.damage;

		auto spawn_blood = [&](double const lost_health) {
			int const n = static_cast<int>(lost_health / target_vitality * 100.0); // 100 is an arbitrary scaling factor.
			for (int i = 0; i < n; ++i) {
				_particles.emplace_back(make_unique<BloodParticle>(position));
			}
		};
		for (Damage::Part const& part : damage.parts()) {
			switch (part.type) {
				case Damage::Part::Type::slash:
					[[fallthrough]];
				case Damage::Part::Type::pierce:
					spawn_blood(part.amount);
					_particles.emplace_back(make_unique<TextParticle>(position, std::to_string(lround(part.amount)), Color::white()));
					sound_manager()[pierce_sound_handle].play();
					break;
				case Damage::Part::Type::cleave:
					[[fallthrough]];
				case Damage::Part::Type::bludgeon:
					spawn_blood(part.amount);
					_particles.emplace_back(make_unique<TextParticle>(position, std::to_string(lround(part.amount)), Color::white()));
					sound_manager()[hit_sound_handle].play();
					break;
				case Damage::Part::Type::burn:
					_particles.emplace_back(make_unique<TextParticle>(position, std::to_string(lround(part.amount)), Color::orange()));
					break;
				case Damage::Part::Type::freeze:
					_particles.emplace_back(make_unique<TextParticle>(position, std::to_string(lround(part.amount)), Color::cyan()));
					break;
				case Damage::Part::Type::blight:
					_particles.emplace_back(make_unique<TextParticle>(position, std::to_string(lround(part.amount)), Color::black()));
					break;
			}
		}
	}

	void WorldRenderer::visit(LightningBoltEffect const& e)
	{
		static auto lightning_bolt_sound_handle = sound_manager().add("resources/sounds/spells/lightning-bolt.wav");

		GamePoint position = Layout::dflt().to_world(e.origin());
		for (int i = 0; i < 35; ++i) {
			_particles.emplace_back(make_unique<YellowMagicParticle>(position));
		}
		sound_manager()[lightning_bolt_sound_handle].play();
	}
}
