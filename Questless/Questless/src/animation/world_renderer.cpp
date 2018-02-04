//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/world_renderer.hpp"

#include "animation/entity_animator.hpp"
#include "animation/particles/arrow_particle.hpp"
#include "animation/particles/blood_particle.hpp"
#include "animation/particles/green_magic_particle.hpp"
#include "animation/particles/text_particle.hpp"
#include "animation/particles/yellow_magic_particle.hpp"
#include "animation/tile_texturer.hpp"
#include "game.hpp"
#include "utility/utility.hpp"
#include "world/region.hpp"

using namespace sdl;
using namespace units;

namespace ql
{
	std::optional<still> world_renderer::_unknown_entity_animation;

	initializer<world_renderer> _initializer;
	void world_renderer::initialize()
	{
		auto unknown_entity_animation_ss = the_texture_manager().add("resources/textures/entities/unknown.png");
		_unknown_entity_animation = still{unknown_entity_animation_ss, texture_space::vector::zero()};
	}

	world_renderer::world_renderer(world_view const& world_view)
		: _world_view{&world_view}
		, _tile_selector_animation
			{ ql::sprite_animation
				{ smake<ql::sprite_sheet>
					( the_texture_manager().add("resources/textures/terrain/selector.png")
					, sprite_sheet_space::vector{4, 1}
					)
				, std::vector<sprite_animation::frame>
					{ {game_space::seconds{0.100}, sprite_sheet_space::point{0, 0}, texture_space::vector{0, 0}}
					, {game_space::seconds{0.075}, sprite_sheet_space::point{1, 0}, texture_space::vector{0, 0}}
					, {game_space::seconds{0.050}, sprite_sheet_space::point{2, 0}, texture_space::vector{0, 0}}
					, {game_space::seconds{0.025}, sprite_sheet_space::point{3, 0}, texture_space::vector{0, 0}}
					, {game_space::seconds{0.050}, sprite_sheet_space::point{2, 0}, texture_space::vector{0, 0}}
					, {game_space::seconds{0.075}, sprite_sheet_space::point{1, 0}, texture_space::vector{0, 0}}
					}
				, looping{true}
				}
			}
		, _terrain_render_is_current{false}
	{}

	auto world_renderer::get_entity_id_var(entity_cref_var_t entity) -> entity_id_var_t
	{
		struct visitor
		{
			entity_id_var_t operator ()(being const& being) { return being.id; }
			entity_id_var_t operator ()(object const& object) { return object.id; }
		};
		return std::visit(visitor{}, entity);
	}

	auto world_renderer::get_entity_cref_var(entity_id_var_t id) -> entity_cref_var_t
	{
		struct visitor
		{
			entity_cref_var_t operator ()(ql::id<being> being_id) { return the_game().beings.cref(being_id); }
			entity_cref_var_t operator ()(ql::id<object> object_id) { return the_game().objects.cref(object_id); }
		};
		return std::visit(visitor{}, id);
	}

	entity const* world_renderer::get_entity_cptr(entity_id_var_t id)
	{
		struct visitor
		{
			entity const* operator ()(ql::id<being> id) { return the_game().beings.cptr(id); }
			entity const* operator ()(ql::id<object> id) { return the_game().objects.cptr(id); }
		};
		return std::visit(visitor{}, id);
	}

	void world_renderer::update_view(world_view const& world_view, std::vector<sptr<effect>> effects)
	{
		_world_view = &world_view;
		_terrain_render_is_current = false;

		for (auto const& effect : effects) {
			effect->accept(*this);
		}

		//! @todo Should the tile texture and entity animation caches ever be cleaned out? If so, when and how? Consider tracking how long since a texture was last used and purging it after a set time.
		_entity_animation_map.clear();
	}

	void world_renderer::update()
	{
		// Update tile selector animation.
		_tile_selector_animation.update();

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

	void world_renderer::draw_terrain()
	{
		// Render if necessary.
		if (!_terrain_render_is_current) {
			render_terrain();
			_terrain_render_is_current = true;
		}
		// Draw terrain.
		if (_terrain_texture) {
			the_game().camera().draw(*_terrain_texture, game_space::point{center(_terrain_bounds)});
		}
		// Draw tile highlights, if active.
		if (_highlight_predicate) {
			int const visual_range = _world_view->visual_range();
			for (int q = -visual_range; q <= visual_range; ++q) {
				for (int r = -visual_range; r <= visual_range; ++r) {
					region_tile::vector const offset{q, r};
					if (offset.length() <= visual_range) {
						auto tile_coords = _world_view->origin() + offset;
						if ((*_highlight_predicate)(tile_coords)) {
							_tile_selector_animation.draw
								( layout::dflt().to_world(tile_coords)
								, the_game().camera()
								);
						}
					}
				}
			}
		}
	}

	void world_renderer::draw_entities()
	{
		// Created a view of the entities sorted by y-coordinates.
		auto y_sort = [](world_view::entity_view const& a, world_view::entity_view const& b) {
			if (entity const* entity_a = get_entity_cptr(a.id)) {
				if (entity const* entity_b = get_entity_cptr(b.id)) {
					double const y_a = layout::dflt().to_world(entity_a->coords).y();
					double const y_b = layout::dflt().to_world(entity_b->coords).y();
					return y_a > y_b;
				}
			}
			return false;
		};
		std::multiset<world_view::entity_view, decltype(y_sort)> sorted_entity_views
			( _world_view->entity_views().begin()
			, _world_view->entity_views().end()
			, y_sort
			);

		// Draw from the sorted view.
		for (auto const& entity_view : sorted_entity_views) {
			// Attempt to load the entity.
			if (entity const* entity = get_entity_cptr(entity_view.id)) {
				auto entity_var_ref = get_entity_cref_var(entity_view.id);

				auto& entity_animation = get_animation(entity_view.id);

				float intensity = static_cast<float>((entity_view.perception.level - perception::minimum_level) / (perception::maximum_level - perception::minimum_level));
				switch (entity_view.perception.category()) {
					case perception::category::none:
						break;
					case perception::category::low:
						_unknown_entity_animation->draw
							( layout::dflt().to_world(entity->coords)
							, the_game().camera()
							, colors::color_vector{intensity, intensity, intensity, 1.0f}
							);
						break;
					case perception::category::medium:
					case perception::category::high:
					case perception::category::full:
					{
						// Draw heading.
						struct heading_drawer
						{
							void operator ()(being const& being)
							{
								game_space::point start = layout::dflt().to_world(being.coords);
								game_space::point end = layout::dflt().to_world(being.coords.neighbor(being.direction));
								the_game().camera().draw_lines({start, end}, colors::magenta());
							}
							void operator ()(object const&) {}
						};
						std::visit(heading_drawer{}, entity_var_ref);

						//! @todo Use the following if the overload helpers in utility.h can be repaired.
						//std::visit
						//	( overload
						//		( [](being const& being)
						//			{
						//				game_space::point start = layout::dflt().to_world(being.coords);
						//				game_space::point end = layout::dflt().to_world(being.coords.neighbor(being.direction));
						//				the_game().camera().draw_lines({start, end}, color::magenta());
						//			}
						//		, [](object const&) {}
						//		)
						//	, entity_var_ref
						//	);

						entity_animation.draw
							( layout::dflt().to_world(entity->coords)
							, the_game().camera()
							, colors::color_vector{intensity, intensity, intensity, 1.0f}
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

	void world_renderer::draw_effects()
	{
		for (auto& animation_and_coords : _animations) {
			auto const& animation = *animation_and_coords.first;
			animation.draw(animation_and_coords.second, the_game().camera());
		}
	}

	void world_renderer::set_highlight_predicate(std::function<bool(region_tile::point)> predicate)
	{
		_highlight_predicate = std::move(predicate);
		_terrain_render_is_current = false;
	}

	void world_renderer::clear_highlight_predicate()
	{
		_highlight_predicate = std::nullopt;
		_terrain_render_is_current = false;
	}

	texture& world_renderer::cache_tile_texture(tile const& tile)
	{
		tile_texturer tile_texturer;
		tile.accept(tile_texturer);
		_tile_textures[tile.subtype()] = tile_texturer.texture();
		return *_tile_textures[tile.subtype()];
	};

	animation& world_renderer::cache_animation(entity_id_var_t entity_id)
	{
		entity_animator entity_animator;
		get_entity_cptr(entity_id)->accept(entity_animator);

		auto result = _entity_animation_map.insert(std::make_pair(entity_id, std::move(entity_animator.animation())));
		return *result.first->second;
	};

	animation& world_renderer::get_animation(entity_id_var_t entity_id)
	{
		// Search for the entity's animation in the cache.
		auto it = _entity_animation_map.find(entity_id);
		// If it's there, use it. Otherwise, create and cache the animation.
		return it != _entity_animation_map.end()
			? *it->second
			: cache_animation(entity_id);
	}

	void world_renderer::render_terrain()
	{
		std::optional<game_space::box> opt_bounds = _world_view->bounds();
		if (!opt_bounds) {
			_terrain_texture = nullptr;
			return;
		}
		_terrain_bounds = *opt_bounds;
		
		_terrain_texture = umake<texture>(window_space::vector{lround(width(_terrain_bounds)), lround(height(_terrain_bounds))});
		_terrain_texture->as_target([&] {
			the_renderer().clear(colors::clear());
			for (auto const& section_view : _world_view->section_views()) {
				auto opt_section = _world_view->region().section_at(section_view.coords);
				if (opt_section) {
					ql::section const& section = *opt_section;
					for (int q = 0; q < section::diameter; ++q) {
						for (int r = 0; r < section::diameter; ++r) {
							section_tile::point section_tile_coords{q, r};
							perception tile_perception = section_view.tile_perceptions[section_tile_coords.q][section_tile_coords.r];
							if (tile_perception.category() != perception::category::none) {
								region_tile::point const region_tile_coords = section.region_tile_coords(section_tile_coords);
								game_space::point const tile_game_point = layout::dflt().to_world(region_tile_coords);
								game_space::point const terrain_game_point = _terrain_bounds.position;
								window_space::point const tile_screen_point
									{ lround(tile_game_point.x() - terrain_game_point.x())
									, lround(terrain_game_point.y() - tile_game_point.y() + height(_terrain_bounds) - 1)
									};

								// Get the current tile.
								tile const& tile = section.tile_at(section_tile_coords);
								// Search for its texture in the cache.
								auto it = _tile_textures.find(tile.subtype());
								// If it's there, use it. Otherwise, create the texture and cache it.
								texture& tile_texture = it != _tile_textures.end() ? *it->second : cache_tile_texture(tile);

								float const intensity = static_cast<float>((tile_perception.level - perception::minimum_level) / (perception::maximum_level - perception::minimum_level));
								tile_texture.draw_transformed
									( tile_screen_point
									, texture_space::vector::zero()
									, colors::color_vector{intensity, intensity, intensity, 1.0f}
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

	void world_renderer::visit(arrow_attack_effect const& e)
	{
		static auto arrow_sound_handle = the_sound_manager().add("resources/sounds/spells/eagle-eye.wav");

		game_space::point source = layout::dflt().to_world(e.origin());
		game_space::point target = layout::dflt().to_world(e.target);
		_animations.push_back(std::make_pair(umake<arrow_particle>(source, target), source));
		the_sound_manager()[arrow_sound_handle].play();
	}

	void world_renderer::visit(eagle_eye_effect const& e)
	{
		static auto eagle_eye_sound_handle = the_sound_manager().add("resources/sounds/spells/eagle-eye.wav");

		game_space::point position = layout::dflt().to_world(e.origin());
		for (int i = 0; i < 50; ++i) {
			_animations.push_back(std::make_pair(umake<green_magic_particle>(), position));
		}
		the_sound_manager()[eagle_eye_sound_handle].play();
	}

	void world_renderer::visit(injury_effect const& e)
	{
		static auto pierce_sound_handle = the_sound_manager().add("resources/sounds/weapons/pierce.wav");
		static auto hit_sound_handle = the_sound_manager().add("resources/sounds/weapons/hit.wav");

		being const* const target_being = the_game().beings.cptr(e.target_being_id);
		body_part const* const target_part = target_being ? target_being->body.find_part(e.target_part_id) : nullptr;
		double const target_vitality = target_part ? target_part->vitality.value() : 100.0; // Assume vitality = 100 if being no longer exists to check.
		//! @todo Pass along the vitality in the event object if it's needed here (to avoid having to make up a number).

		game_space::point const position = layout::dflt().to_world(e.origin());

		dmg::group const& damage = e.damage;

		for (auto const& part : damage.parts()) {
			struct damage_renderer
			{
				std::vector<std::pair<uptr<animation>, units::game_space::point>>& animations;
				game_space::point const position;
				double const target_vitality;

				void spawn_blood(double const lost_health)
				{
					constexpr double scaling_factor = 20.0;
					int const n = static_cast<int>(lost_health / target_vitality * scaling_factor);
					for (int i = 0; i < n; ++i) {
						animations.push_back(std::make_pair(umake<blood_particle>(), position));
					}
				};

				void render_slash_or_pierce(double const amount)
				{
					spawn_blood(amount);
					animations.push_back(std::make_pair
						( umake<text_particle>(std::to_string(lround(amount)), colors::white())
						, position
						));
					the_sound_manager()[pierce_sound_handle].play();
				}

				void render_cleave_or_bludgeon(double const amount)
				{
					spawn_blood(amount);
					animations.push_back(std::make_pair
						( umake<text_particle>(std::to_string(lround(amount)), colors::white())
						, position
						));
					the_sound_manager()[hit_sound_handle].play();
				}

				void operator ()(dmg::slash const& slash) { render_slash_or_pierce(slash.value()); }
				void operator ()(dmg::pierce const& pierce) { render_slash_or_pierce(pierce.value()); }
				void operator ()(dmg::cleave const& cleave) { render_cleave_or_bludgeon(cleave.value()); }
				void operator ()(dmg::bludgeon const& bludgeon) { render_cleave_or_bludgeon(bludgeon.value()); }
				void operator ()(dmg::burn const& burn)
				{
					animations.push_back(std::make_pair
						( umake<text_particle>(std::to_string(lround(burn.value())), colors::orange())
						, position
						));
				}
				void operator ()(dmg::freeze const& freeze)
				{
					animations.push_back(std::make_pair
						( umake<text_particle>(std::to_string(lround(freeze.value())), colors::cyan())
						, position
						));
				}
				void operator ()(dmg::blight const& blight)
				{
					animations.push_back(std::make_pair
						( umake<text_particle>(std::to_string(lround(blight.value())), colors::black())
						, position
						));
				}
				void operator ()(dmg::poison const& poison)
				{
					animations.push_back(std::make_pair
						( umake<text_particle>(std::to_string(lround(poison.value())), colors::purple())
						, position
						));
				}
				void operator ()(dmg::shock const& shock)
				{
					animations.push_back(std::make_pair
						( umake<text_particle>(std::to_string(lround(shock.value())), colors::yellow())
						, position
						));
				}
			};
			std::visit(damage_renderer{_animations, position, target_vitality}, part);
		}
	}

	void world_renderer::visit(lightning_bolt_effect const& e)
	{
		static auto lightning_bolt_sound_handle = the_sound_manager().add("resources/sounds/spells/lightning-bolt.wav");

		game_space::point position = layout::dflt().to_world(e.origin());
		for (int i = 0; i < 35; ++i) {
			_animations.push_back(std::make_pair(umake<yellow_magic_particle>(), position));
		}
		the_sound_manager()[lightning_bolt_sound_handle].play();
	}
}
