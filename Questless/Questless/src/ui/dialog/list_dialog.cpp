//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#include "list_dialog.hpp"

#include "rsrc/fonts.hpp"

#include <cassert>

namespace ql {
	using namespace view::literals;

	namespace {
		constexpr auto title_height = 40.0_px;
		constexpr auto option_height = 20.0_px;
		constexpr view::vector padding{10.0_px, 10.0_px};
	}

	list_dialog::list_dialog( //
		widget* parent,
		rsrc::fonts const& fonts,
		sf::String const& title,
		std::vector<std::tuple<sf::String, std::function<void()>>> options)
		: _parent{parent}
		, _title{title, fonts.dumbledor1, 28, sf::Color::Black} //
	{
		assert(!options.empty());

		_bg.setOutlineThickness(1.0f);
		_bg.setOutlineColor(sf::Color::Black);
		_bg.setFillColor(sf::Color{255, 192, 153});

		// Initialize background size.
		auto bg_size = [&]() -> view::vector {
			auto const width{_title.get_size()[0]};
			view::px const height{static_cast<float>(_options.size()) * option_height};
			return {width, height};
		}();

		// Create options.
		for (auto& [string, callback] : options) {
			label option_label{string, fonts.dumbledor1, 20};
			option_label.set_outline_color(sf::Color::Black);
			option_label.set_outline_thickness(2.0f);
			// Update background to fit this option's text.
			bg_size[0] = std::max(bg_size[0], option_label.get_size()[0]);
			// Move new option into options list.
			_options.emplace_back(std::move(option_label), std::move(callback));
		}

		// Add padding to background size.
		bg_size += 2.0f * padding;
		// Pass background size to background shape.
		_bg.setSize(view::to_sfml(bg_size));

		// Highlight first option.
		std::get<0>(_options.front()).set_fill_color(sf::Color::White);
	}

	auto list_dialog::get_size() const -> view::vector {
		return _size;
	}

	auto list_dialog::update(sec /*elapsed_time*/) -> void {}

	auto list_dialog::set_position(view::point position) -> void {
		_position = position;
		confine_to_parent();
	}

	auto list_dialog::get_position() const -> view::point {
		return _position;
	}

	auto list_dialog::on_parent_resize(view::vector parent_size) -> void {
		_parent_size = parent_size;
		confine_to_parent();
	}

	auto list_dialog::on_key_press(sf::Event::KeyEvent const& event) -> event_handled {
		switch (event.code) {
			// Cancel dialog.
			case sf::Keyboard::Backspace:
				[[fallthrough]];
			case sf::Keyboard::Escape:
				_promise.set_value(std::nullopt);
				break;
			// Choose current selection.
			case sf::Keyboard::Enter:
				[[fallthrough]];
			case sf::Keyboard::Space:
				choose();
				break;
			// Select by key.
			case sf::Keyboard::Num1:
				select(0);
				break;
			case sf::Keyboard::Num2:
				select(1);
				break;
			case sf::Keyboard::Num3:
				select(2);
				break;
			case sf::Keyboard::Num4:
				select(3);
				break;
			case sf::Keyboard::Num5:
				select(4);
				break;
			case sf::Keyboard::Num6:
				select(5);
				break;
			case sf::Keyboard::Num7:
				select(6);
				break;
			case sf::Keyboard::Num8:
				select(7);
				break;
			case sf::Keyboard::Num9:
				select(8);
				break;
			case sf::Keyboard::Num0:
				select(9);
				break;
			// Move selection up/down.
			case sf::Keyboard::Up: {
				int const options_count = static_cast<int>(_options.size());
				select((_selection - 1 + options_count) % options_count);
				break;
			}
			case sf::Keyboard::Down: {
				int const options_count = static_cast<int>(_options.size());
				select((_selection + 1) % options_count);
				break;
			}
			default:
				return event_handled::no;
		}
		return event_handled::yes;
	}

	auto list_dialog::on_mouse_press(sf::Event::MouseButtonEvent const& event) -> event_handled {
		if (event.button == sf::Mouse::Left) {
			choose();
			return event_handled::yes;
		} else {
			return event_handled::no;
		}
	}

	auto list_dialog::on_mouse_wheel_scroll(sf::Event::MouseWheelScrollEvent const& event) -> event_handled {
		select(_selection + event.y);
		return event_handled::no;
	}

	auto list_dialog::draw(sf::RenderTarget& target, sf::RenderStates states) const -> void {
		// Draw background.
		target.draw(_bg);
		// Draw title.
		target.draw(_title);
		// Draw options.
		for (auto const& option : _options) {
			target.draw(std::get<0>(option), states);
		}
	}

	auto list_dialog::select(int selection) -> void {
		if (selection < static_cast<int>(_options.size())) {
			int const previous_selection = _selection;
			_selection = selection;
			if (_selection != previous_selection) {
				// Selection changed. Update colors.
				std::get<0>(_options[previous_selection]).set_fill_color(sf::Color::Black);
				std::get<0>(_options[_selection]).set_fill_color(sf::Color::White);
			}
		}
	}

	auto list_dialog::choose() -> void {
		_promise.set_value(std::move(std::get<1>(_options[_selection])));
	}

	auto list_dialog::confine_to_parent() -> void {
		{ // Move up/left if list dialog goes past the parent's bounding box.
			auto parent_box = _parent->get_bounding_box();
			if (_position[0] + _size[0] > right(parent_box)) { _position[0] -= _size[0]; }
			if (_position[1] + _size[1] > bottom(parent_box)) { _position[1] -= _size[1]; }
		}

		// Set background position.
		_bg.setPosition(view::to_sfml(_position));

		auto content_position = _position + padding;
		// Set title position.
		_title.set_position(content_position);
		// Set option positions.
		content_position[1] += title_height;
		for (auto& option : _options) {
			std::get<0>(option).set_position(content_position);
			content_position[1] += option_height;
		}
	}
}
