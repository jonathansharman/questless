//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animators.hpp"

#include "animation.hpp"
#include "bleeding.hpp"
#include "flame.hpp"
#include "scene_node.hpp"
#include "still_image.hpp"
#include "still_shape.hpp"

#include "entities/beings/body.hpp"
#include "entities/beings/body_part.hpp"
#include "entities/objects/campfire.hpp"
#include "items/magic/gatestone.hpp"
#include "items/magic/scroll.hpp"
#include "items/weapons/bow.hpp"
#include "items/weapons/quarterstaff.hpp"
#include "items/weapons/quiver.hpp"
#include "magic/spell.hpp"
#include "rsrc/entity.hpp"
#include "rsrc/item.hpp"
#include "rsrc/particle.hpp"
#include "rsrc/spell.hpp"
#include "rsrc/tile.hpp"
#include "utility/utility.hpp"
#include "utility/visitation.hpp"
#include "world/terrain.hpp"

namespace ql {
	uptr<animation> animate_tile(rsrc::tile const& resources, ent tile_id) {
		switch (reg.get<terrain>(tile_id)) {
			case terrain::dirt:
				return umake<still_image>(resources.txtr.dirt);
			case terrain::edge:
				//! @todo Edge tiles should simply not be drawn.
				return umake<still_image>(resources.txtr.blank);
			case terrain::grass:
				return umake<still_image>(resources.txtr.grass);
			case terrain::sand:
				return umake<still_image>(resources.txtr.sand);
			case terrain::snow:
				return umake<still_image>(resources.txtr.snow);
			case terrain::stone:
				return umake<still_image>(resources.txtr.stone);
			case terrain::water:
				return umake<still_image>(resources.txtr.water);
			default:
				UNREACHABLE;
		}
	}

	uptr<animation> animate(rsrc::spell const& resources, magic::spell const& spell) {
		return match(
			spell.value,
			[&](magic::telescope const&) { return umake<still_image>(resources.txtr.telescope); },
			[&](magic::heal const&) { return umake<still_image>(resources.txtr.heal); },
			[&](magic::shock const&) { return umake<still_image>(resources.txtr.shock); },
			[&](magic::teleport const&) { return umake<still_image>(resources.txtr.teleport); });
	}

	uptr<animation> animate_entity(rsrc::entity const& entity_resources, rsrc::particle const& particle_resources, ent entity_id) {
		if (reg.has<campfire>(entity_id)) {
			auto firewood = umake<still_image>(entity_resources.txtr.firewood);
			firewood->set_relative_origin({0.5f, 0.5f});
			auto scene_node = umake<ql::scene_node>(std::move(firewood));

			auto flame = umake<ql::flame>(particle_resources);
			// Pre-update the flame so it's steady immediately.
			flame->update(2.0_s);

			scene_node->front_children.push_back(umake<ql::scene_node>(std::move(flame)));
			return scene_node;
		} else if (auto body = reg.try_get<ql::body>(entity_id)) {
			// Sprite animation
			auto scene_node = umake<ql::scene_node>(umake<ql::sprite_animation>( //
				ql::sprite_sheet{entity_resources.ss.human, {3, 1}},
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
				auto bleeding = umake<ql::bleeding>(particle_resources, severity * conversion_factor);
				scene_node->front_children.push_back(std::move(bleeding));
			}
			return scene_node;
		}
		// Above cases are exhaustive.
		UNREACHABLE;
	}

	uptr<animation> animate_item(rsrc::item const& item_resources, rsrc::spell const& spell_resources, ent item_id) {
		if (reg.has<bow>(item_id)) {
			return umake<still_image>(item_resources.bow);
		} else if (reg.has<quarterstaff>(item_id)) {
			return umake<still_image>(item_resources.quarterstaff);
		} else if (reg.has<quiver>(item_id)) {
			return umake<still_image>(item_resources.quiver);
		} else if (auto scroll = reg.try_get<ql::scroll>(item_id)) {
			if (scroll->spell == std::nullopt) {
				return umake<still_image>(item_resources.blank_scroll);
			} else {
				auto node = umake<scene_node>(umake<still_image>(item_resources.written_scroll));
				node->front_children.push_front(animate(spell_resources, *scroll->spell));
				return node;
			}
		} else if (auto gatestone = reg.try_get<ql::gatestone>(item_id)) {
			if (gatestone->charge.value() == 0_mp) {
				return umake<still_image>(item_resources.uncharged_gatestone);
			} else {
				// Create gatestone still.
				auto gatestone_still = umake<still_image>(item_resources.charged_gatestone);
				sf::Color const draw_color_factor = [&] {
					sf::Color result;
					switch (gatestone->color) {
						case magic::color::white:
							result = sf::Color::White;
							break;
						case magic::color::black:
							result = sf::Color{51, 51, 51};
							break;
						case magic::color::green:
							result = sf::Color::Green;
							break;
						case magic::color::red:
							result = sf::Color::Red;
							break;
						case magic::color::blue:
							result = sf::Color::Blue;
							break;
						case magic::color::yellow:
							result = sf::Color::Yellow;
							break;
						case magic::color::violet:
							result = sf::Color{192, 0, 255};
							break;
						case magic::color::orange:
							result = sf::Color{255, 192, 0};
							break;
						default:
							UNREACHABLE;
					}
					result.a = static_cast<sf::Uint8>(
						(255 * gatestone->charge.value() / gatestone->charge.upper_bound()).value);
					return result;
				}();
				gatestone_still->set_color(draw_color_factor);

				// Create scene node with gatestone still.
				auto node = umake<scene_node>(std::move(gatestone_still));

				//! @todo Clean up the charge bar. Remove magic numbers.

				{ // Overlay charge bar background.
					auto charge_bar_background = umake<sf::RectangleShape>(sf::Vector2f{6, 55});
					charge_bar_background->setFillColor(sf::Color::Black);
					node->front_children.push_front(umake<still_shape>(std::move(charge_bar_background)));
				}

				{ // Overlay charge bar foreground.
					float const height = 55.f * gatestone->charge.value().value / gatestone->charge.upper_bound().value;
					auto charge_bar_foreground = umake<sf::RectangleShape>(sf::Vector2f{6, height});
					charge_bar_foreground->setFillColor(draw_color_factor);
					charge_bar_foreground->setOutlineColor(sf::Color::Black);
					charge_bar_foreground->setOutlineThickness(1);
					node->front_children.push_front(umake<still_shape>(std::move(charge_bar_foreground)));
				}

				return node;
			}
		}
		// Above cases are exhaustive.
		UNREACHABLE;
	}
}
