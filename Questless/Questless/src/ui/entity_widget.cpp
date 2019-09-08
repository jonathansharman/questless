//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

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
		rsrc::entity const& entity_resources,
		rsrc::particle const& particle_resources,
		world_view::entity_view entity_view)
		: _entity_resources{entity_resources}
		, _particle_resources{particle_resources}
		, _ev{entity_view} //
	{
		_ani = [this]() -> uptr<animation> {
			if (_ev.perception >= 25_perception) {
				if (reg.has<campfire>(_ev.id)) {
					auto firewood = umake<still_image>(_entity_resources.txtr.firewood);
					firewood->set_relative_origin({0.5f, 0.5f}, true);
					auto scene_node = umake<ql::scene_node>(std::move(firewood));

					auto flame = umake<ql::flame>(_particle_resources);
					// Pre-update the flame so it's steady immediately.
					flame->update(2.0_s);

					scene_node->front_children.push_back(umake<ql::scene_node>(std::move(flame)));
					return scene_node;
				} else if (auto body = reg.try_get<ql::body>(_ev.id)) {
					// Sprite animation
					auto scene_node = umake<ql::scene_node>(umake<sprite_animation>( //
						ql::sprite_sheet{_entity_resources.ss.human, {3, 1}},
						std::vector<sprite_animation::frame>{//
							{0.2_s, {0, 0}, {0, 12}},
							{0.2_s, {1, 0}, {0, 12}},
							{0.2_s, {2, 0}, {0, 12}},
							{0.2_s, {1, 0}, {0, 12}}},
						sprite_animation::loop_type::looping,
						sprite_animation::start_time::random));

					// Bleeding animation
					auto total_bleeding = 0.0_blood_per_tick;
					body->for_all_parts([&](body_part const& part) { total_bleeding += part.stats.bleeding.cur; });
					if (total_bleeding > 0.0_blood_per_tick) {
						// Severity of bleeding is the rate of blood loss over the being's base vitality.
						auto const severity = total_bleeding / body->stats.a.vitality.base;
						// Converts the severity of bleeding to drops of animated blood per second.
						constexpr auto conversion_factor = ql::bleeding::drops{5.0} / 1.0_s / (1.0_blood_per_tick / 1_hp);
						auto bleeding = umake<ql::bleeding>(_particle_resources, severity * conversion_factor);
						scene_node->front_children.push_back(std::move(bleeding));
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
