/**
* @file    Dialog.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Dialog abstract base class.
*/

#ifndef DIALOG_H
#define DIALOG_H

#include "sdl-wrappers/Renderable.h"
#include "sdl-wrappers/Window.h"
#include "sdl-wrappers/Input.h"
#include "utility/Initializer.h"

namespace questless
{
	class Dialog : public sdl::Renderable
	{
	public:
		Dialog() : _closed{false} {}

		/// @return Whether the player has completed or closed the dialog.
		/// @note update() and draw() should not be called on a closed dialog.
		bool closed() const { return _closed; }

		/// Sets the dialog to closed.
		void close() { _closed = true; }

		/// Updates the dialog state based on input.
		/// @param input User input used to update the dialog.
		virtual void update(const sdl::Input& input) = 0;

		/// Draws the dialog to the screen.
		/// @param window The window.
		virtual void draw(const sdl::Window& window) = 0;
	protected:
		static constexpr int _prompt_top = 40;
	private:
		friend class Initializer<Dialog>;
		static Initializer<Dialog> _initializer;
		static void initialize();

		bool _closed;
	};
}

#endif