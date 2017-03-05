/**
* @file    MagnitudeDialog.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the MagnitudeDialog class, which allows the player to choose a floating-point value.
*/

#pragma once

#include <string>
#include <functional>

#include <boost/optional.hpp>

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
			, boost::optional<double> min
			, boost::optional<double> max
			, std::function<void(boost::optional<double>)> cont
			)
			: _title{std::move(title)}
			, _prompt{std::move(prompt)}
			, _magnitude{default}
			, _min{min}
			, _max{max}
			, _cont{std::move(cont)}
		{
			load_textures();
		}

		State update() override;

		void draw() const override;
	private:
		std::string _title;
		std::string _prompt;
		double _magnitude;
		boost::optional<double> _min;
		boost::optional<double> _max;
		Continuation<boost::optional<double>> _cont;

		sdl::Texture::ptr _txt_title;
		sdl::Texture::ptr _txt_prompt;
		sdl::Texture::ptr _txt_selector;

		void refresh() override { load_textures(); }

		void load_textures();
	};
}
