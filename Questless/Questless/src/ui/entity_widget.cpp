//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "entity_widget.hpp"

#include "animation/animation.hpp"
#include "animation/bleeding.hpp"
#include "animation/flame.hpp"
#include "animation/scene_node.hpp"
#include "animation/sprite_animation.hpp"
#include "animation/still_image.hpp"
#include "entities/beings/body.hpp"
#include "entities/beings/body_part.hpp"
#include "entities/objects/campfire.hpp"
#include "reg.hpp"
#include "rsrc/entity.hpp"
#include "rsrc/particle.hpp"

namespace ql {
	entity_widget::entity_widget( //
		reg& reg,
		rsrc::entity const& entity_resources,
		rsrc::particle const& particle_resources,
		world_view::entity_view entity_view)
		: _reg{&reg}
		, _entity_resources{entity_resources}
		, _particle_resources{particle_resources}
		, _ev{entity_view} //
	{
		_ani = [this]() -> uptr<animation> {
			if (_ev.perception >= 25_perception) {
				if (_reg->has<campfire>(_ev.id)) {
					auto firewood = umake<still_image>(_entity_resources.txtr.firewood);
					firewood->set_relative_origin({0.5f, 0.5f}, true);

					auto ani = umake<scene_node>(std::move(firewood));

					auto flame = umake<ql::flame>(_particle_resources);
					{ // Pre-update the flame so it's steady immediately.
						constexpr auto fast_forward = 2.0_s;
						constexpr int step = 100;
						for (int i = 0; i < step; ++i) {
							flame->update(fast_forward / step);
						}
					}

					ani->front_children.push_back(std::move(flame));

					return ani;
				} else if (auto body = _reg->try_get<ql::body>(_ev.id)) {
					// Sprite animation
					auto scene_node = umake<ql::scene_node>(umake<sprite_animation>( //
						ql::sprite_sheet{_entity_resources.ss.human, {3, 1}},
						std::vector<sprite_animation::frame>{//
							{0.4_s, {0, 0}, {14, 28}},
							{0.4_s, {1, 0}, {14, 28}},
							{0.4_s, {2, 0}, {14, 28}},
							{0.4_s, {1, 0}, {14, 28}}},
						sprite_animation::loop_type::looping,
						sprite_animation::start_time::random));

					// Bleeding animation
					auto total_bleeding = 0.0_blood_per_tick;
					body->for_all_parts([&](body_part const& part) { total_bleeding += part.stats.bleeding.cur; });
					if (total_bleeding > 0.0_blood_per_tick) {
						// Severity of bleeding is the rate of blood loss over the being's base vitality.
						auto const severity = total_bleeding / body->stats.a.vitality.base;
						// Converts the severity of bleeding to drops of animated blood per second.
						constexpr auto conversion_factor = bleeding::drops{5.0} / 1.0_s / (1.0_blood_per_tick / 1_hp);
						scene_node->front_children.push_back(
							umake<bleeding>(_particle_resources, severity * conversion_factor));
					}
					return scene_node;
				}
			}
			return umake<still_image>(_entity_resources.txtr.unknown);
		}();
	}

	auto entity_widget::get_size() const -> view::vector {
		return {};
	}

	auto entity_widget::update(sec elapsed_time) -> void {
		_ani->update(elapsed_time);
	}

	auto entity_widget::set_position(view::point position) -> void {
		_position = position;
		_ani->setPosition(to_sfml(_position));
	}

	auto entity_widget::get_position() const -> view::point {
		return _position;
	}

	auto entity_widget::draw(sf::RenderTarget& target, sf::RenderStates states) const -> void {
		//! @todo Use a shader to indicate perception.
		target.draw(*_ani, states);
	}
}
