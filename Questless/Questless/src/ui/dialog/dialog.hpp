//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "ui/input_manager.hpp"
#include "ui/label.hpp"
#include "ui/widget.hpp"

#include <SFML/System/String.hpp>

#include <future>
#include <optional>

namespace ql {
	namespace rsrc {
		struct fonts;
	}

	//! A UI element used to exchange information with the player.
	template <typename T>
	struct dialog : widget {
		dialog(widget* parent, rsrc::fonts const& fonts) : widget{parent}, _fonts{fonts} {}

		virtual ~dialog() = default;

		std::future<std::optional<T>> get_future() {
			return _promise.get_future();
		}

	protected:
		rsrc::fonts const& _fonts;

		std::promise<std::optional<T>> _promise;

		static constexpr int _title_font_size = 32;
		static constexpr int _prompt_font_size = 20;
		static constexpr int _selector_font_size = 20;

		static constexpr int _prompt_top = 40;
		static constexpr int _selector_top = 100;

		label make_title(sf::String const& title,
			sf::Color fill_color = sf::Color::White,
			sf::Color outline_color = sf::Color::Black);

		label make_prompt(sf::String const& prompt,
			sf::Color fill_color = sf::Color::White,
			sf::Color outline_color = sf::Color::Black);

		label make_selector(sf::String const& selector,
			sf::Color fill_color = sf::Color::White,
			sf::Color outline_color = sf::Color::Black);
	};
}
