/**
* @file    Dialog.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Dialog abstract base class.
*/

#pragma once

#include "sdl-wrappers/Renderable.h"
#include "sdl-wrappers/resources.h"
#include "sdl-wrappers/Window.h"
#include "sdl-wrappers/Input.h"
#include "utility/Initializer.h"

namespace questless
{
	class Dialog : public sdl::Renderable
	{
	public:
		enum class State {open, closed};

		using ptr = std::unique_ptr<Dialog>;

		/// Updates the dialog state based on input.
		/// @param input User input used to update the dialog.
		/// @return True if the dialog is resolved after this update or false if it's still active.
		virtual bool update(sdl::Input& input) = 0; /// @todo Take input by ref or const-ref?

		/// Draws the dialog to the screen.
		/// @param window The window.
		virtual void draw(const sdl::Window& window) = 0;
	protected:
		template <typename... Args>
		class Continuation
		{
		public:
			Continuation(std::function<void(Args...)> cont) : _cont{ std::move(cont) } {}

			bool operator ()(Args&&... args)
			{
				_cont(std::forward<Args>(args)...);
				return true;
			}
		private:
			std::function<void(Args...)> _cont;
		};

		//template <>
		//class Continuation<void>
		//{
		//public:
		//	Continuation(std::function<void()> cont) : _cont{ std::move(cont) } {}

		//	bool operator ()()
		//	{
		//		_cont();
		//		return true;
		//	}
		//private:
		//	std::function<void()> _cont;
		//};

		static constexpr int _prompt_top = 40;

		static sdl::Handle<sdl::Font> title_font_handle() { return _title_font_handle; }
		static sdl::Handle<sdl::Font> prompt_font_handle() { return _prompt_font_handle; }
		static sdl::Handle<sdl::Font> io_font_handle() { return _io_font_handle; }
		static sdl::Handle<sdl::Font> list_option_font_handle() { return _list_option_font_handle; }
	private:
		friend class Initializer<Dialog>;
		static Initializer<Dialog> _initializer;
		static void initialize();

		static sdl::Handle<sdl::Font> _title_font_handle;
		static sdl::Handle<sdl::Font> _prompt_font_handle;
		static sdl::Handle<sdl::Font> _io_font_handle;
		static sdl::Handle<sdl::Font> _list_option_font_handle;
	};
}
