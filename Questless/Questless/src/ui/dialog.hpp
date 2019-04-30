//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "input_manager.hpp"
#include "label.hpp"
#include "widget.hpp"

#include "quantities/wall_time.hpp"
#include "utility/reference.hpp"

#include <functional>

namespace ql {
	namespace rsrc {
		struct fonts;
	}

	//! A UI element used to exchange information with the player.
	struct dialog : widget {
		//! @param window The window in which this dialog will be displayed.
		dialog(sf::Window const& window, rsrc::fonts const& fonts) : _window{window}, _fonts{fonts} {}

		virtual ~dialog() = default;

		//! Whether this dialog is open.
		bool open() const {
			return _open;
		}

		//! Whether this dialog is closed.
		bool closed() const {
			return !_open;
		}

		//! Closes this dialog.
		void close() {
			_open = false;
		}

	protected:
		template <typename... Args>
		struct continuation {
			continuation(std::function<void(Args...)> cont) : _cont{std::move(cont)} {}

			void operator()(Args&&... args) {
				_cont(std::forward<Args>(args)...);
			}

		private:
			std::function<void(Args...)> _cont;
		};

		sf::Window const& _window;
		rsrc::fonts const& _fonts;

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

	private:
		bool _open = true;
	};
}
