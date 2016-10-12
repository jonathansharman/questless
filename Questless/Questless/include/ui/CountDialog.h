/**
* @file    CountDialog.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the CountDialog class.
*/

#ifndef COUNT_DIALOG_CONTROLLER_H
#define COUNT_DIALOG_CONTROLLER_H

#include <string>
#include <functional>

#include "Dialog.h"

namespace questless
{
	class CountDialog : public Dialog
	{
	public:
		CountDialog
			( std::string title
			, std::string prompt
			, int default
			, boost::optional<int> min
			, boost::optional<int> max
			, std::function<bool(int)> predicate
			, std::function<void(boost::optional<int>)> cont
			)
			: _title{std::move(title)}
			, _prompt{std::move(prompt)}
			, _count{default}
			, _min{min}
			, _max{max}
			, _predicate{std::move(predicate)}
			, _cont{std::move(cont)}
		{}

		/// Updates the dialog state based on input.
		/// @param input User input used to update the dialog.
		void update(const sdl::Input& input) override;

		/// Draws the dialog to the screen.
		/// @param window The window.
		void draw(const sdl::Window& window) override;
	private:
		std::string _title;
		std::string _prompt;
		int _count;
		boost::optional<int> _min;
		boost::optional<int> _max;
		std::function<bool(int)> _predicate;
		std::function<void(boost::optional<int>)> _cont;

		sdl::Texture::ptr _txt_title;
		sdl::Texture::ptr _txt_prompt;
		sdl::Texture::ptr _txt_current;

		void refresh() override;
	};
}

#endif
