//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "hotbar.hpp"

#include "rsrc/utility.hpp"

namespace ql {
	using namespace view::literals;

	namespace {
		constexpr auto interslot_gap = 2.0_px;
		constexpr auto bottom_gap = 2.0_px;
		constexpr auto slot_size = view::vector{65.0_px, 65.0_px};
		constexpr auto slot_h_padding = 5.0_px;
		constexpr auto slot_v_padding = 5.0_px;
	}

	hotbar::hotbar(widget& parent)
		: widget{&parent}
		, _slot_texture{rsrc::load<sf::Texture>("resources/textures/ui/hud/hotbar-slot.png")} //
	{}

	view::vector hotbar::get_size() const {
		view::px const width = static_cast<float>(_item_count) * (slot_size[0] + slot_h_padding) + slot_size[0];
		view::px const height = slot_size[1] + slot_v_padding;
		return {width, height};
	}

	void hotbar::update(sec /*elapsed_time*/, std::vector<sf::Event>& events) {
		//! @todo Update item widgets.

		for (auto event : events) {
			switch (event.type) {
				case sf::Event::KeyPressed:
					switch (event.key.code) {
						case sf::Keyboard::Space:
							add(_most_recent_selection);
							break;
						case sf::Keyboard::Num1:
							add(0);
							break;
						case sf::Keyboard::Num2:
							add(1);
							break;
						case sf::Keyboard::Num3:
							add(2);
							break;
						case sf::Keyboard::Num4:
							add(3);
							break;
						case sf::Keyboard::Num5:
							add(4);
							break;
						case sf::Keyboard::Num6:
							add(5);
							break;
						case sf::Keyboard::Num7:
							add(6);
							break;
						case sf::Keyboard::Num8:
							add(7);
							break;
						case sf::Keyboard::Num9:
							add(8);
							break;
						case sf::Keyboard::Num0:
							add(9);
							break;
						default:
							break;
					}
				default:
					break;
			}
		}
	}

	void hotbar::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		auto const layout = get_bounding_box();

		sf::Sprite slot_sprite{_slot_texture};
		for (size_t i = 0; i < _o_item_ids.size(); ++i) {
			view::px const x = layout.position[0] + (slot_size[0] + interslot_gap) * static_cast<float>(i);
			view::px const y = layout.position[1];
			slot_sprite.setPosition(view::to_sfml(view::point{x, y}));
			target.draw(slot_sprite, states);
			//! @todo Draw items.
		}
	}

	std::vector<int> hotbar::poll_selections() {
		auto result = std::move(_selections);
		_selections.clear();
		return result;
	}

	auto hotbar::most_recent_selection() const {
		return _most_recent_selection;
	}

	void hotbar::add(int selection) {
		_selections.push_back(selection);
		_most_recent_selection = selection;
	}
}
