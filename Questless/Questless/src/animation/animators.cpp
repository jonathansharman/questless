//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "animators.hpp"

#include "animation.hpp"
#include "bleeding.hpp"
#include "flame.hpp"
#include "scene_node.hpp"
#include "sprite_animation.hpp"
#include "still_image.hpp"
#include "still_shape.hpp"

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
#include "utility/utility.hpp"
#include "utility/visitation.hpp"

namespace ql {
	uptr<animation> animate(rsrc::spell const& resources, magic::spell const& spell) {
		return match(
			spell.value,
			[&](magic::telescope const&) { return umake<still_image>(resources.txtr.telescope); },
			[&](magic::heal const&) { return umake<still_image>(resources.txtr.heal); },
			[&](magic::shock const&) { return umake<still_image>(resources.txtr.shock); },
			[&](magic::teleport const&) { return umake<still_image>(resources.txtr.teleport); });
	}

	uptr<animation> animate_item(rsrc::item const& item_resources, rsrc::spell const& spell_resources, id item_id) {
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
