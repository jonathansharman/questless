//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "world_renderer.hpp"

#include "animators.hpp"
#include "entity_animator.hpp"
#include "particles/arrow_particle.hpp"
#include "particles/blood_particle.hpp"
#include "particles/green_magic_particle.hpp"
#include "particles/text_particle.hpp"
#include "particles/yellow_magic_particle.hpp"
#include "still_image.hpp"

#include "damage/damage.hpp"
#include "entities/beings/being.hpp"
#include "entities/beings/world_view.hpp"
#include "entities/objects/object.hpp"
#include "game.hpp"
#include "rsrc/fonts.hpp"
#include "utility/utility.hpp"
#include "utility/visitation.hpp"
#include "world/region.hpp"
#include "world/tile.hpp"

namespace ql {
	world_renderer::world_renderer(rsrc::fonts const& fonts, world_view const& world_view)
		: _fonts{fonts}, _world_view{&world_view} {}

	auto world_renderer::get_entity_id_var(entity_cref_var_t entity) -> entity_id_var_t {
		struct visitor {
			entity_id_var_t operator()(ql::being const& being) {
				return being.id;
			}
			entity_id_var_t operator()(ql::object const& object) {
				return object.id;
			}
		};
		return std::visit(visitor{}, entity);
	}

	auto world_renderer::get_entity_cref_var(entity_id_var_t id) -> entity_cref_var_t {
		struct visitor {
			entity_cref_var_t operator()(ql::id<ql::being> being_id) {
				return the_game().beings.cref(being_id);
			}
			entity_cref_var_t operator()(ql::id<ql::object> object_id) {
				return the_game().objects.cref(object_id);
			}
		};
		return std::visit(visitor{}, id);
	}

	entity const* world_renderer::get_entity_cptr(entity_id_var_t id) {
		struct visitor {
			entity const* operator()(ql::id<ql::being> id) {
				return the_game().beings.cptr(id);
			}
			entity const* operator()(ql::id<ql::object> id) {
				return the_game().objects.cptr(id);
			}
		};
		return std::visit(visitor{}, id);
	}

	void world_renderer::update_view(world_view const& world_view, std::vector<effects::effect> const& effects) {
		_world_view = &world_view;

		for (auto const& effect : effects) {
			match(effect,
				[&](effects::arrow_attack const& e) {
					world::point source = to_world(e.origin);
					world::point target = to_world(e.target);
					_effect_animations.push_back(umake<arrow_particle>(source, target));
					_resources.sfx.arrow.play();
				},
				[&](effects::injury const& e) {
					being const* const target_being = the_game().beings.cptr(e.target_being_id);
					body_part const* const target_part = target_being ? target_being->body.find_part(e.target_part_id) : nullptr;
					// Assume vitality = 100 if being no longer exists to check.
					auto const target_vitality = target_part ? target_part->stats.a.vitality.value() : 100.0_hp;
					//! @todo Pass along the vitality in the event object if it's needed here (to avoid having to make
					//! up a number).

					world::point const position = to_world(e.origin);

					dmg::group const& damage = e.damage;

					for (auto const& part : damage.parts()) {
						static constexpr sec text_duration = 2.0_s;

						auto& font = _fonts.firamono;

						auto spawn_blood = [&](double const damage) {
							constexpr double scaling_factor = 20.0;
							int const n = static_cast<int>(damage / target_vitality.value * scaling_factor);
							for (int i = 0; i < n; ++i) {
								_effect_animations.push_back(umake<blood_particle>());
							}
						};

						auto render_slash_or_pierce = [&](double const amount) {
							spawn_blood(amount);
							_effect_animations.push_back(umake<text_particle>(
								position, text_duration, font, std::to_string(lround(amount)), sf::Color::White));
							_resources.sfx.pierce.play();
						};

						auto render_cleave_or_bludgeon = [&](double const amount) {
							spawn_blood(amount);
							_effect_animations.push_back(umake<text_particle>(
								position, text_duration, font, std::to_string(lround(amount)), sf::Color::White));
							_resources.sfx.hit.play();
						};

						match(part, //
							[&](dmg::slash const& slash) { render_slash_or_pierce(slash.value); },
							[&](dmg::pierce const& pierce) { render_slash_or_pierce(pierce.value); },
							[&](dmg::cleave const& cleave) { render_cleave_or_bludgeon(cleave.value); },
							[&](dmg::bludgeon const& bludgeon) { render_cleave_or_bludgeon(bludgeon.value); },
							[&](dmg::burn const& burn) {
								_effect_animations.push_back(umake<text_particle>(
									position, text_duration, font, std ::to_string(lround(burn.value)), sf::Color{255, 128, 0}));
							},
							[&](dmg::freeze const& freeze) {
								_effect_animations.push_back(umake<text_particle>(
									position, text_duration, font, std::to_string(lround(freeze.value)), sf::Color::Cyan));
							},
							[&](dmg::blight const& blight) {
								_effect_animations.push_back(umake<text_particle>(
									position, text_duration, font, std::to_string(lround(blight.value)), sf::Color::Black));
							},
							[&](dmg::poison const& poison) {
								_effect_animations.push_back(umake<text_particle>(
									position, text_duration, font, std::to_string(lround(poison.value)), sf::Color{128, 0, 192}));
							},
							[&](dmg::shock const& shock) {
								_effect_animations.push_back(umake<text_particle>(
									text_duration, position, font, std::to_string(lround(shock.value)), sf::Color::Yellow));
							});
					}
				},
				[&](effects::lightning_bolt const& e) {
					world::point position = to_world(e.origin);
					for (int i = 0; i < 35; ++i) {
						auto p = umake<yellow_magic_particle>();
						p->setPosition(to_sfml(position));
						_effect_animations.push_back(std::move(p));
					}
					_resources.sfx.shock.play();
				},
				[&](effects::telescope const& e) {
					world::point position = to_world(e.origin);
					for (int i = 0; i < 50; ++i) {
						auto particle = umake<green_magic_particle>();
						particle->setPosition(to_sfml(position));
						_effect_animations.push_back(std::move(particle));
					}
					_resources.sfx.telescope.play();
				});
		}
	}

	void world_renderer::update(sec elapsed_time) {
		// Update tile selector animation.
		_tile_resources.ani.selector.update(elapsed_time);

		// Update tile animations.
		for (auto& id_and_animation : _tile_animations) {
			id_and_animation.second->update(elapsed_time);
		}

		// Update entity animations.
		for (auto& id_and_animation : _entity_animations) {
			id_and_animation.second->update(elapsed_time);
		}

		// Update effect animations.
		for (std::size_t i = 0; i < _effect_animations.size();) {
			_effect_animations[i]->update(elapsed_time);
			if (_effect_animations[i]->stopped()) {
				_effect_animations.erase(_effect_animations.begin() + i);
			} else {
				++i;
			}
		}
	}

	void world_renderer::draw_terrain(sf::RenderTarget& target, sf::RenderStates states) const {
		for (auto const& section_view : _world_view->section_views()) {
			section const* section = _world_view->region().section_at(section_view.coords);
			if (!section) continue;
			for (span q = 0_span; q < section::diameter; ++q) {
				for (span r = 0_span; r < section::diameter; ++r) {
					section_tile::point const section_tile_coords{q, r};

					perception::level tile_perception =
						section_view.tile_perceptions[section_tile_coords.q.value][section_tile_coords.r.value];

					if (perception::get_category(tile_perception) != perception::category::none) {
						//! @todo Use time_game_point or remove these lines if no longer needed.
						region_tile::point const region_tile_coords = section->region_tile_coords(section_tile_coords);
						world::point const tile_game_point = to_world(region_tile_coords);

						// Get the current tile.
						tile const& tile = section->tile_at(section_tile_coords);
						// Search for its animation in the cache.
						auto it = _tile_animations.find(tile.subtype());
						// If it's there, use it. Otherwise, create and cache.
						animation& tile_animation = it != _tile_animations.end() ? *it->second : cache_tile_animation(tile);

						auto const perception_above_minimum = tile_perception - perception::minimum_level;
						auto const perception_range = perception::maximum_level - perception::minimum_level;
						sf::Uint8 const intensity = to_uint8(perception_above_minimum / perception_range);

						//! @todo Use draw_color.
						sf::Color const draw_color{intensity, intensity, intensity};

						target.draw(tile_animation, states);
					}
				}
			}
		}

		// Draw tile highlights, if active.
		if (_highlight_predicate) {
			span const visual_range = _world_view->visual_range();
			for (span q = -visual_range; q <= visual_range; ++q) {
				for (span r = -visual_range; r <= visual_range; ++r) {
					region_tile::vector const offset{q, r};
					if (offset.length() <= visual_range) {
						auto tile_coords = _world_view->origin() + offset;
						if ((*_highlight_predicate)(tile_coords)) {
							_tile_resources.ani.selector.setPosition(to_sfml(to_world(tile_coords)));
							_tile_resources.ani.selector.draw(target, states);
						}
					}
				}
			}
		}
	}

	void world_renderer::draw_entities(sf::RenderTarget& target, sf::RenderStates states) const {
		// Created a view of the entities sorted by y-coordinates.
		auto y_sort = [](world_view::entity_view const& a, world_view::entity_view const& b) {
			if (entity const* entity_a = get_entity_cptr(a.id)) {
				if (entity const* entity_b = get_entity_cptr(b.id)) {
					auto const y_a = y(to_world(entity_a->coords));
					auto const y_b = y(to_world(entity_b->coords));
					return y_a > y_b;
				}
			}
			return false;
		};
		std::multiset<world_view::entity_view, decltype(y_sort)> sorted_entity_views(
			_world_view->entity_views().begin(), _world_view->entity_views().end(), y_sort);

		// Draw from the sorted view.
		for (auto const& entity_view : sorted_entity_views) {
			// Attempt to load the entity.
			if (entity const* entity = get_entity_cptr(entity_view.id)) {
				auto entity_var_ref = get_entity_cref_var(entity_view.id);

				auto& entity_animation = get_animation(entity_view.id);

				auto const perception_above_minimum = entity_view.perception.value() - perception::minimum_level;
				auto const perception_range = perception::maximum_level - perception::minimum_level;
				sf::Uint8 const intensity = to_uint8(perception_above_minimum / perception_range);

				switch (perception::get_category(entity_view.perception)) {
					case perception::category::none:
						break;
					case perception::category::low:
						_entity_resources.ani.unknown.setPosition(to_sfml(to_world(entity->coords)));
						_entity_resources.ani.unknown.set_color({intensity, intensity, intensity});
						_entity_resources.ani.unknown.draw(target, states);
						break;
					case perception::category::medium:
					case perception::category::high:
					case perception::category::full: {
						// Draw heading.
						match(entity_var_ref,
							[&](being const& being) {
								sf::Vertex line[] = {//
									{to_sfml(to_world(being.coords)), sf::Color::Magenta},
									{to_sfml(to_world(being.coords.neighbor(being.direction))), sf::Color::Magenta}};
								target.draw(line, sizeof(line), sf::PrimitiveType::Lines);
							},
							[&](object const&) {});

						entity_animation.setPosition(to_sfml(to_world(entity->coords)));
						entity_animation.setColor(sf::Color{intensity, intensity, intensity});
						target.draw(entity_animation, states);
						break;
					}
					default:
						assert(false && "Invalid perception category.");
				}
			} else {
				// Remove the being from the animation cache if it doesn't exist anymore.
				_entity_animations.erase(entity_view.id);
			}
		}
	}

	void world_renderer::draw_effects(sf::RenderTarget& target, sf::RenderStates states) const {
		for (auto const& animation : _effect_animations) {
			target.draw(*animation, states);
		}
	}

	void world_renderer::set_highlight_predicate(std::function<bool(region_tile::point)> predicate) {
		_highlight_predicate = std::move(predicate);
	}

	void world_renderer::clear_highlight_predicate() {
		_highlight_predicate = std::nullopt;
	}

	animation& world_renderer::cache_tile_animation(tile const& tile) {
		_tile_animations[tile.subtype()] = animate(_tile_resources, tile);
		return *_tile_animations[tile.subtype()];
	};

	animation& world_renderer::cache_entity_animation(entity_id_var_t entity_id) {
		entity_animator entity_animator{_entity_resources};
		get_entity_cptr(entity_id)->accept(entity_animator);

		auto result = _entity_animations.insert(std::make_pair(entity_id, std::move(entity_animator.animation)));
		return *result.first->second;
	};

	animation& world_renderer::get_animation(entity_id_var_t entity_id) const {
		// Search for the entity's animation in the cache.
		auto it = _entity_animations.find(entity_id);
		// If it's there, use it. Otherwise, create and cache the animation.
		return it != _entity_animations.end() ? (*it)->second : cache_entity_animation(entity_id);
	}
}
