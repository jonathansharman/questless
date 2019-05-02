//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animators.hpp"
#include "items/magic/gatestone.hpp"
#include "items/scroll.hpp"
#include "rsrc/item.hpp"
#include "scene_node.hpp"
#include "still_image.hpp"
#include "still_shape.hpp"

namespace ql {
	void item_animator::visit(bow const&) {
		animation = umake<still_image>(item_resources.bow);
	}

	void item_animator::visit(quarterstaff const&) {
		animation = umake<still_image>(item_resources.quarterstaff);
	}

	void item_animator::visit(quiver const&) {
		animation = umake<still_image>(item_resources.quiver);
	}

	void item_animator::visit(scroll const& scroll) {
		if (scroll.blank()) {
			animation = umake<still_image>(item_resources.blank_scroll);
		} else {
			auto node = umake<scene_node>(umake<still_image>(item_resources.written_scroll));
			node->front_children.push_front(animate(spell_resources, scroll.spell()));

			animation = std::move(node);
		}
	}

	void item_animator::visit(gatestone const& gatestone) {
		if (gatestone.charge == 0.0_mp) {
			animation = umake<still_image>(item_resources.uncharged_gatestone);
		} else {
			// Create gatestone still.
			auto gatestone_still = umake<still_image>(item_resources.charged_gatestone);
			sf::Color const draw_color_factor = [&] {
				sf::Color result;
				switch (gatestone.color()) {
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
						//! @todo Replace with precondition when available.
						assert(false && "Invalid spell color.");
				}
				result.a = static_cast<sf::Uint8>(255 * (gatestone.charge.value() / gatestone.capacity()).value);
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
				float const charge_pct = static_cast<float>((gatestone.charge.value() / gatestone.capacity()).value);
				auto charge_bar_foreground = umake<sf::RectangleShape>(sf::Vector2f{6, 55 * charge_pct});
				charge_bar_foreground->setFillColor(draw_color_factor);
				charge_bar_foreground->setOutlineColor(sf::Color::Black);
				charge_bar_foreground->setOutlineThickness(1);
				node->front_children.push_front(umake<still_shape>(std::move(charge_bar_foreground)));
			}

			animation = std::move(node);
		}
	}
}
