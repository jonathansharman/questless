//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "sdl/Renderable.h"
#include "sdl/resources.h"
#include "sdl/Window.h"
#include "sdl/Input.h"
#include "utility/Initializer.h"
#include "utility/reference.h"

namespace questless
{
	//! A UI element used to exchange information with the player.
	class Dialog : public sdl::Renderable
	{
	public:
		enum class State { open, closed };

		//! Updates the dialog state based on input. To be called once per frame as long as it's open.
		//! @return The state of the dialog after this update: either open or closed.
		virtual State update() = 0; //! @todo Take input by ref or const-ref?

		//! Draws the dialog to the screen.
		virtual void draw() const = 0;
	protected:
		template <typename... Args>
		class Continuation
		{
		public:
			Continuation(std::function<void(Args...)> cont) : _cont{ std::move(cont) } {}

			State operator ()(Args&&... args)
			{
				_cont(std::forward<Args>(args)...);
				return State::closed;
			}
		private:
			std::function<void(Args...)> _cont;
		};

		static constexpr int _title_font_size = 32;
		static constexpr int _prompt_font_size = 20;
		static constexpr int _selector_font_size = 20;

		static constexpr int _prompt_top = 40;
		static constexpr int _selector_top = 100;

		static uptr<sdl::Texture> make_title(char const* title, sdl::Color color = sdl::Color::white());
		static uptr<sdl::Texture> make_prompt(char const* prompt, sdl::Color color = sdl::Color::white());
		static uptr<sdl::Texture> make_selector(char const* selector, sdl::Color color = sdl::Color::white());

		static void draw_title(sdl::Texture const& title);
		static void draw_prompt(sdl::Texture const& prompt);
		static void draw_selector(sdl::Texture const& selector);
	};
}
