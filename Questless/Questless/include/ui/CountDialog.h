/**
* @file    CountDialog.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the CountDialog class.
*/

#pragma once

#include <string>
#include <functional>
#include <optional>

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
			, std::optional<int> min
			, std::optional<int> max
			, std::function<void(std::optional<int>)> cont
			)
			: _title{std::move(title)}
			, _prompt{std::move(prompt)}
			, _count{default}
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
		int _count;
		std::optional<int> _min;
		std::optional<int> _max;
		Continuation<std::optional<int>> _cont;

		sdl::Texture::ptr _txt_title;
		sdl::Texture::ptr _txt_prompt;
		sdl::Texture::ptr _txt_selector;

		void refresh() override { load_textures(); }

		void load_textures();
	};
}
