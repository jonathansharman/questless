//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "list_dialog.hpp"

#include "rsrc/fonts.hpp"

#include <cassert>

namespace ql {
	using namespace view::literals;

	list_dialog::list_dialog( //
		rsrc::fonts const& fonts,
		sf::String title,
		std::vector<std::tuple<sf::String, std::function<void()>>> options)
		: _title{title, fonts.dumbledor1, 28} //
	{
		assert(!options.empty());

		constexpr view::vector padding{10.0_px, 10.0_px};

		_title.setFillColor(sf::Color::Black);

		_bg.setOutlineThickness(1.0f);
		_bg.setOutlineColor(sf::Color::Black);
		_bg.setFillColor(sf::Color{255, 192, 153});

		float bg_width = _title.getLocalBounds().width;

		// Create options.
		for (auto& [string, callback] : options) {
			sf::Text option_text{string, fonts.dumbledor1, 20};
			option_text.setFillColor(sf::Color::Black);
			option_text.setOutlineColor(sf::Color::Black);
			option_text.setOutlineThickness(2.0f);
			// Update background to fit this option's text.
			bg_width = std::max(bg_width, option_text.getLocalBounds().width);
			// Move new option into options list.
			_options.emplace_back(std::move(option_text), std::move(callback));
		}

		// Highlight first option.
		std::get<0>(_options.front()).setFillColor(sf::Color::White);

		// Add width padding to background and set size.
		bg_width += 2.0f * padding[0];
		_bg.setSize({bg_width, _title_height + static_cast<int>(_options.size()) * _option_height + 2 * padding.y});

		// Confine bounds to window.
		auto bg_bounds = _bg.getGlobalBounds();
		if (bg_bounds.left + bg_bounds.width > _window.getSize().x) { _bg.move(-bg_bounds.width, 0); }
		if (bg_bounds.top + bg_bounds.height > _window.getSize().y) { _bg.move(0, -bg_bounds.height); }

		// Position contents relative to background.
		_title.setPosition(_bg.getPosition() + padding);
		for (std::size_t i = 0; i < _options.size(); ++i) {
			std::get<0>(_options[i])
				.setPosition(
					_bg.getPosition() + padding + sf::Vector2f{0, _title_height + static_cast<float>(i) * _option_height});
		}
	}

	auto list_dialog::get_size() const -> view::vector {
		return view::vector_from_sfml(_bg.getSize());
	}

	void list_dialog::update(sec /*elapsed_time*/) {
		int const option_count = static_cast<int>(_options.size());

		if (!_options.empty()) {
			int const old_selection = _selection;

			_selection = _selection % option_count;
			_selection = _selection < 0 ? _selection + option_count : _selection;

			if (old_selection != _selection) {
				// Selection changed. Update colors.
				std::get<0>(_options[old_selection]).setFillColor(sf::Color::Black);
				std::get<0>(_options[_selection]).setFillColor(sf::Color::White);
			}
		}
	}

	void list_dialog::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		// Draw background.
		target.draw(_bg);
		// Draw title.
		target.draw(_title);
		// Draw options.
		for (auto const& option : _options) {
			target.draw(option, states);
		}
	}

	auto list_dialog::set_position(view::point position) -> void {
		_position = position;
	}

	auto list_dialog::get_position() const -> view::point {
		return _position;
	}

	auto list_dialog::on_parent_resize(view::vector parent_size) -> void {
		_bg.setPosition(origin);
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
			case sf::Keyboard::Up:
				int const options_count = static_cast<int>(_options.size());
				select((_selection - 1 + options_count) % options_count);
				break;
			case sf::Keyboard::Down:
				int const options_count = static_cast<int>(_options.size());
				select((_selection + 1) % options_count);
				break;
			default:
				return event_handled::no;
		}
		return event_handled::yes;
	}

	auto list_dialog::on_mouse_press(sf::Event::MouseButtonEvent const& event) -> event_handled {
		if (event.button == sf::Mouse::Left) {
			select(_selection, true);
			return event_handled::yes;
		} else {
			return event_handled::no;
		}
	}

	auto list_dialog::on_mouse_wheel_scroll(sf::Event::MouseWheelScrollEvent const& event) -> event_handled {
		select(_selection + event.y);
	}

	auto list_dialog::select(int selection) -> void {
		if (selection < _options.size()) {
			int const previous_selection = _selection;
			_selection = selection;
			if (_selection != previous_selection) {}
		}
	}

	auto list_dialog::choose() -> void {
		_promise.set_value(std::move(_options[_selection]));
	}
}
