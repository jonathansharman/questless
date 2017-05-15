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
	std::optional<Still> WorldRenderer::_unknown_entity_animation;

	Initializer<WorldRenderer> _initializer;
	void WorldRenderer::initialize()
	{
		auto unknown_entity_animation_ss = texture_manager().add("resources/textures/entities/unknown.png");
		_unknown_entity_animation = Still{unknown_entity_animation_ss, TexturePoint{0, 0}};
	}

	auto WorldRenderer::get_entity_id_var(entity_cref_var_t entity) -> entity_id_var_t
	{
		struct Visitor
		{
			entity_id_var_t operator ()(Being const& being) { return being.id; }
			entity_id_var_t operator ()(Object const& object) { return object.id; }
		};
		return std::visit(Visitor{}, entity);
	}

	auto WorldRenderer::get_entity_cref_var(entity_id_var_t id) -> entity_cref_var_t
	{
		struct Visitor
		{
			entity_cref_var_t operator ()(Id<Being> being_id) { return game().beings.cref(being_id); }
			entity_cref_var_t operator ()(Id<Object> object_id) { return game().objects.cref(object_id); }
		};
		return std::visit(Visitor{}, id);
	}

	Entity const* WorldRenderer::get_entity_cptr(entity_id_var_t id)
	{
		struct Visitor
		{
			Entity const* operator ()(Id<Being> id) { return game().beings.cptr(id); }
			Entity const* operator ()(Id<Object> id) { return game().objects.cptr(id); }
		};
		return std::visit(Visitor{}, id);
	}

	void WorldRenderer::update_view(WorldView const& world_view, std::vector<sptr<Effect>> effects)
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
		// Update cached entity animations.

		for (auto& id_and_animation : _entity_animation_map) {
			id_and_animation.second->update();
		}

		// Update effect sounds and animations.

		for (std::size_t i = 0; i < _animations.size();) {
			_animations[i].first->update();
			if (_animations[i].first->over()) {
				_animations.erase(_animations.begin() + i);
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

	void WorldRenderer::draw_entities()
	{
		for (auto const& entity_view : _world_view->entity_views()) {
			// Attempt to load the entity.
			if (Entity const* entity = get_entity_cptr(entity_view.id)) {
				auto entity_var_ref = get_entity_cref_var(entity_view.id);

				auto& entity_animation = get_animation(entity_view.id);

				uint8_t intensity = percentage_to_byte((entity_view.perception.level - Perception::minimum_level) / (Perception::maximum_level - Perception::minimum_level));
				switch (entity_view.perception.category()) {
					case Perception::Category::none:
						break;
					case Perception::Category::low:
						_unknown_entity_animation->draw
						(Layout::dflt().to_world(entity->coords)
							, game().camera()
							, Color{intensity, intensity, intensity}
						);
						break;
					case Perception::Category::medium:
					case Perception::Category::high:
					case Perception::Category::full:
					{
						// Draw heading.
						struct HeadingDrawer
						{
							void operator ()(Being const& being)
							{
								GamePoint start = Layout::dflt().to_world(being.coords);
								GamePoint end = Layout::dflt().to_world(being.coords.neighbor(being.direction));
								game().camera().draw_lines({start, end}, Color::magenta());
							}
							void operator ()(Object const&) {}
						};
						std::visit(HeadingDrawer{}, entity_var_ref);

						//! @todo Use the following if the overload helpers in utility.h can be repaired.
						//std::visit
						//	( overload
						//		( [](Being const& being)
						//			{
						//				GamePoint start = Layout::dflt().to_world(being.coords);
						//				GamePoint end = Layout::dflt().to_world(being.coords.neighbor(being.direction));
						//				game().camera().draw_lines({start, end}, Color::magenta());
						//			}
						//		, [](Object const&) {}
						//		)
						//	, entity_var_ref
						//	);

						entity_animation.draw
						(Layout::dflt().to_world(entity->coords)
							, game().camera()
							, Color{intensity, intensity, intensity}
						);
						break;
					}
					default:
						throw std::logic_error{"Invalid perception category."};
				}
			} else {
				// Remove the being from the animation cache if it doesn't exist anymore.
				_entity_animation_map.erase(entity_view.id);
			}
		}
	}

	void WorldRenderer::draw_effects()
	{
		for (auto& animation_and_coords : _animations) {
			auto const& animation = *animation_and_coords.first;
			animation.draw(animation_and_coords.second, game().camera());
		}
	}

	void WorldRenderer::set_highlight_predicate(std::function<bool(RegionTile::Point)> predicate)
	{
		_highlight_predicate = std::move(predicate);
		_terrain_render_is_current = false;
	}

	void WorldRenderer::clear_highlight_predicate()
	{
		_highlight_predicate = std::nullopt;
		_terrain_render_is_current = false;
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

	Animation& WorldRenderer::cache_animation(entity_id_var_t entity_id)
	{
		EntityAnimator entity_animator;
		get_entity_cptr(entity_id)->accept(entity_animator);

		auto result = _entity_animation_map.insert(std::make_pair(entity_id, std::move(entity_animator.animation())));
		return *result.first->second;
	};

	Animation& WorldRenderer::get_animation(entity_id_var_t entity_id)
	{
		// Search for the entity's animation in the cache.
		auto it = _entity_animation_map.find(entity_id);
		// If it's there, use it. Otherwise, create and cache the animation.
		return it != _entity_animation_map.end()
			? *it->second
			: cache_animation(entity_id);
	}

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
							SectionTile::Point section_tile_coords{q, r};
							Perception tile_perception = section_view.tile_perceptions[section_tile_coords.q][section_tile_coords.r];
							if (tile_perception.category() != Perception::Category::none) {
								RegionTile::Point const region_tile_coords = section.region_tile_coords(section_tile_coords);
								GamePoint const tile_game_point = Layout::dflt().to_world(region_tile_coords);
								GamePoint const terrain_game_point = _terrain_bounds.position();
								ScreenPoint const tile_screen_point
									{ lround(tile_game_point.x - terrain_game_point.x)
									, lround(terrain_game_point.y - tile_game_point.y + _terrain_bounds.h - 1)
									};

								// Get the current tile.
								Tile const& tile = section.tile(section_tile_coords);
								// Search for its texture in the cache.
								auto it = _tile_textures.find(tile.tile_class());
								// If it's there, use it. Otherwise, create the texture and cache it.
								Texture& tile_texture = it != _tile_textures.end() ? *it->second : cache_tile_texture(tile);

								uint8_t intensity = percentage_to_byte((tile_perception.level - Perception::minimum_level) / (Perception::maximum_level - Perception::minimum_level));

								// Apply highlights.
								if (_highlight_predicate) {
									if ((*_highlight_predicate)(region_tile_coords)) {
										intensity = uint8_t{255};
									}
								}

								tile_texture.draw_transformed
									( tile_screen_point
									, std::nullopt // origin
									, Color{intensity, intensity, intensity}
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
			_animations.push_back(std::make_pair(make_unique<GreenMagicParticle>(), position));
		}
		sound_manager()[eagle_eye_sound_handle].play();
	}

	void WorldRenderer::visit(InjuryEffect const& e)
	{
		static auto pierce_sound_handle = sound_manager().add("resources/sounds/weapons/pierce.wav");
		static auto hit_sound_handle = sound_manager().add("resources/sounds/weapons/hit.wav");

		Being const* target = game().beings.cptr(e.target_id);
		double const target_vitality = target ? target->stats.vitality.get() : 100.0; // Assume vitality = 100 if being no longer exists to check.
		//! @todo Pass along the vitality in the event object if it's needed here.

		GamePoint position = Layout::dflt().to_world(e.origin());

		Damage const& damage = e.damage;

		auto spawn_blood = [&](double const lost_health) {
			int const n = static_cast<int>(lost_health / target_vitality * 100.0); // 100 is an arbitrary scaling factor.
			for (int i = 0; i < n; ++i) {
				_animations.push_back(std::make_pair(make_unique<BloodParticle>(), position));
			}
		};
		for (Damage::Part const& part : damage.parts()) {
			switch (part.type) {
				case Damage::Part::Type::slash:
					[[fallthrough]];
				case Damage::Part::Type::pierce:
					spawn_blood(part.amount);
					_animations.push_back(std::make_pair
						( make_unique<TextParticle>(std::to_string(lround(part.amount)), Color::white())
						, position
						));
					sound_manager()[pierce_sound_handle].play();
					break;
				case Damage::Part::Type::cleave:
					[[fallthrough]];
				case Damage::Part::Type::bludgeon:
					spawn_blood(part.amount);
					_animations.push_back(std::make_pair
						( make_unique<TextParticle>(std::to_string(lround(part.amount)), Color::white())
						, position
						));
					sound_manager()[hit_sound_handle].play();
					break;
				case Damage::Part::Type::burn:
					_animations.push_back(std::make_pair
						( make_unique<TextParticle>(std::to_string(lround(part.amount)), Color::orange())
						, position
						));
					break;
				case Damage::Part::Type::freeze:
					_animations.push_back(std::make_pair
						( make_unique<TextParticle>(std::to_string(lround(part.amount)), Color::cyan())
						, position
						));
					break;
				case Damage::Part::Type::blight:
					_animations.push_back(std::make_pair
						( make_unique<TextParticle>(std::to_string(lround(part.amount)), Color::black())
						, position
						));
					break;
			}
		}
	}

	void WorldRenderer::visit(LightningBoltEffect const& e)
	{
		static auto lightning_bolt_sound_handle = sound_manager().add("resources/sounds/spells/lightning-bolt.wav");

		GamePoint position = Layout::dflt().to_world(e.origin());
		for (int i = 0; i < 35; ++i) {
			_animations.push_back(std::make_pair(make_unique<YellowMagicParticle>(), position));
		}
		sound_manager()[lightning_bolt_sound_handle].play();
	}
}
