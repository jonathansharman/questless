//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "item_widget.hpp"

#include "animation/animation.hpp"
#include "animation/bleeding.hpp"
#include "animation/flame.hpp"
#include "animation/scene_node.hpp"
#include "animation/sprite_animation.hpp"
#include "animation/still_image.hpp"
#include "animation/still_shape.hpp"
#include "items/magic/gatestone.hpp"
#include "items/magic/scroll.hpp"
#include "items/weapons/bow.hpp"
#include "items/weapons/quarterstaff.hpp"
#include "items/weapons/quiver.hpp"
#include "magic/spell.hpp"
#include "rsrc/item.hpp"
#include "rsrc/spell.hpp"
#include "utility/visitation.hpp"

namespace ql {
	using namespace view::literals;

	namespace {
		uptr<animation> animate_spell(rsrc::spell const& resources, magic::spell const& spell) {
			return match(
				spell.value,
				[&](magic::telescope const&) { return umake<still_image>(resources.txtr.telescope); },
				[&](magic::heal const&) { return umake<still_image>(resources.txtr.heal); },
				[&](magic::shock const&) { return umake<still_image>(resources.txtr.shock); },
				[&](magic::teleport const&) { return umake<still_image>(resources.txtr.teleport); });
		}
	}

	item_widget::item_widget(rsrc::item const& item_resources, rsrc::spell const& spell_resources)
		: _item_resources{item_resources}, _spell_resources{spell_resources} {}

	item_widget::~item_widget() = default;

	auto item_widget::get_o_item_id() const -> std::optional<id> {
		return _o_item_id;
	}

	auto item_widget::set_o_item_id(std::optional<id> o_item_id) -> void {
		// Assign and nullopt-check before proceeding.
		_o_item_id = o_item_id;
		if (!_o_item_id) { return; }
		id item_id = *_o_item_id;

		// Render item.
		if (reg.has<bow>(item_id)) {
			_ani = umake<still_image>(_item_resources.bow);
		} else if (reg.has<quarterstaff>(item_id)) {
			_ani = umake<still_image>(_item_resources.quarterstaff);
		} else if (reg.has<quiver>(item_id)) {
			_ani = umake<still_image>(_item_resources.quiver);
		} else if (auto scroll = reg.try_get<ql::scroll>(item_id)) {
			if (scroll->spell == std::nullopt) {
				_ani = umake<still_image>(_item_resources.blank_scroll);
			} else {
				auto node = umake<scene_node>(umake<still_image>(_item_resources.written_scroll));
				node->front_children.push_front(animate_spell(_spell_resources, *scroll->spell));
				_ani = std::move(node);
			}
		} else if (auto gatestone = reg.try_get<ql::gatestone>(item_id)) {
			if (gatestone->charge.value() == 0_mp) {
				_ani = umake<still_image>(_item_resources.uncharged_gatestone);
			} else {
				// Create gatestone still.
				auto gatestone_still = umake<still_image>(_item_resources.charged_gatestone);
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

				_ani = std::move(node);
			}
		}

		// Set/reset position, in case item ID was unset last time set_position() was called.
		_ani->setPosition(to_sfml(_position));
	}

	auto item_widget::get_size() const -> view::vector {
		return size;
	}

	auto item_widget::update(sec elapsed_time) -> void {
		if (_ani) { _ani->update(elapsed_time); }
	}

	auto item_widget::draw(sf::RenderTarget& target, sf::RenderStates states) const -> void {
		sf::RectangleShape bg;
		bg.setPosition(view::to_sfml(_position));
		bg.setSize(view::to_sfml(get_size()));
		bg.setOutlineColor(sf::Color::Black);
		bg.setOutlineThickness(1.0f);
		bg.setFillColor(sf::Color{192, 192, 192});
		if (_ani) { target.draw(*_ani, states); }
	}

	auto item_widget::set_position(view::point position) -> void {
		_position = position;
		if (_ani) { _ani->setPosition(to_sfml(_position)); }
	}

	auto item_widget::get_position() const -> view::point {
		return _position;
	}

	auto item_widget::on_mouse_press(sf::Event::MouseButtonEvent const& event) -> event_handled {
		if (_o_item_id && get_bounding_box().contains(view::point_from_mouse_button_event(event))) {
			on_click(_o_item_id);
			return event_handled::yes;
		} else {
			return event_handled::no;
		}
	}
}
