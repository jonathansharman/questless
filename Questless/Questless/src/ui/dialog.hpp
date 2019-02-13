//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "input_manager.hpp"

#include "quantities/wall_time.hpp"
#include "utility/reference.hpp"

#include <SFML/Graphics.hpp>

#include <functional>

namespace ql {
	namespace rsrc {
		struct fonts;
	}

	//! A UI element used to exchange information with the player.
	struct dialog : public sf::Drawable {
		enum class state { open, closed };

		//! @param window The window in which this dialog will be displayed.
		dialog(sf::Window const& window, rsrc::fonts const& fonts) : _window{window}, _fonts{fonts} {}

		//! Updates the dialog state. To be called once per frame as long as this dialog is open.
		//! @param elapsed_time The amount of time since the last update.
		//! @return The state of this dialog after this update: either open or closed.
		virtual state update(sec elapsed_time, input_manager& im) = 0;

		//! Draws the dialog to the screen.
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

	protected:
		template <typename... Args>
		struct continuation {
			continuation(std::function<void(Args...)> cont) : _cont{std::move(cont)} {}

			state operator()(Args&&... args) {
				_cont(std::forward<Args>(args)...);
				return state::closed;
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

		sf::Text make_title(sf::String const& title,
			sf::Color fill_color = sf::Color::White,
			sf::Color outline_color = sf::Color::Black);

		sf::Text make_prompt(sf::String const& prompt,
			sf::Color fill_color = sf::Color::White,
			sf::Color outline_color = sf::Color::Black);

		sf::Text make_selector(sf::String const& selector,
			sf::Color fill_color = sf::Color::White,
			sf::Color outline_color = sf::Color::Black);
	};
}
