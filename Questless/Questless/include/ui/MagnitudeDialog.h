/**
* @file    MagnitudeDialog.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the MagnitudeDialog class, which allows the player to choose a floating-point value.
*/

#ifndef MAGNITUDE_DIALOG_H
#define MAGNITUDE_DIALOG_H

#include <string>
#include <functional>

#include "Dialog.h"

namespace questless
{
	class MagnitudeDialog : public Dialog
	{
	public:
		MagnitudeDialog
			( std::string title
			, std::string prompt
			, double default
			, optional<double> min
			, optional<double> max
			, std::function<bool(double)> predicate
			, std::function<void(optional<double>)> cont
			)
			: _title{std::move(title)}
			, _prompt{std::move(prompt)}
			, _magnitude{default}
			, _min{min}
			, _max{max}
			, _predicate{std::move(predicate)}
			, _cont{std::move(cont)}
		{
			refresh();
		}

		void refresh() override;

		/// Updates the dialog state based on input.
		/// @param input User input used to update the dialog.
		void update(const sdl::Input& input) override;

		/// Draws the dialog to the screen.
		/// @param window The window.
		void draw(const sdl::Window& window) override;
	private:
		std::string _title;
		std::string _prompt;
		double _magnitude;
		optional<double> _min;
		optional<double> _max;
		std::function<bool(double)> _predicate;
		std::function<void(optional<double>)> _cont;

		sdl::Texture::ptr _txt_title;
		sdl::Texture::ptr _txt_prompt;
		sdl::Texture::ptr _txt_current;
	};
}

#endif