//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "sdl/resources.hpp"
#include "sdl/window.hpp"
#include "sdl/input.hpp"
#include "utility/initializer.hpp"
#include "utility/reference.hpp"

namespace ql
{
	//! A UI element used to exchange information with the player.
	class dialog
	{
	public:
		enum class state { open, closed };

		//! Updates the dialog state based on input. To be called once per frame as long as it's open.
		//! @return The state of the dialog after this update: either open or closed.
		virtual state update() = 0; //! @todo Take input by ref or const-ref?

		//! Draws the dialog to the screen.
		virtual void draw() const = 0;
	protected:
		template <typename... Args>
		class continuation
		{
		public:
			continuation(std::function<void(Args...)> cont) : _cont{ std::move(cont) } {}

			state operator ()(Args&&... args)
			{
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

		static uptr<sdl::texture> make_title(char const* title, units::colors::color color = units::colors::white());
		static uptr<sdl::texture> make_prompt(char const* prompt, units::colors::color color = units::colors::white());
		static uptr<sdl::texture> make_selector(char const* selector, units::colors::color color = units::colors::white());

		static void draw_title(sdl::texture const& title);
		static void draw_prompt(sdl::texture const& prompt);
		static void draw_selector(sdl::texture const& selector);
	};
}
