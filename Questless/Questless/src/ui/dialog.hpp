//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <functional>

#include "sdl/texture.hpp"
#include "utility/reference.hpp"

namespace ql {
	//! A UI element used to exchange information with the player.
	class dialog {
	public:
		enum class state { open, closed };

		//! Updates the dialog state. To be called once per frame as long as this dialog is open.
		//! @return The state of this dialog after this update: either open or closed.
		virtual state update() = 0;

		//! Draws the dialog to the screen.
		virtual void draw() const = 0;
	protected:
		template <typename... Args>
		class continuation {
		public:
			continuation(std::function<void(Args...)> cont) : _cont{ std::move(cont) } {}

			state operator ()(Args&&... args) {
				_cont(std::forward<Args>(args)...);
				return state::closed;
			}
		private:
			std::function<void(Args...)> _cont;
		};

		static constexpr int _title_font_size = 32;
		static constexpr int _prompt_font_size = 20;
		static constexpr int _selector_font_size = 20;

		static constexpr int _prompt_top = 40;
		static constexpr int _selector_top = 100;

		static uptr<sdl::texture> make_title(char const* title, sdl::spaces::colors::color color = sdl::spaces::colors::white());
		static uptr<sdl::texture> make_prompt(char const* prompt, sdl::spaces::colors::color color = sdl::spaces::colors::white());
		static uptr<sdl::texture> make_selector(char const* selector, sdl::spaces::colors::color color = sdl::spaces::colors::white());

		static void draw_title(sdl::texture const& title);
		static void draw_prompt(sdl::texture const& prompt);
		static void draw_selector(sdl::texture const& selector);
	};
}
