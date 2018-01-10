//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <functional>
#include <optional>

#include "dialog.hpp"

namespace ql
{
	//! Retrieves a floating-point value from the player.
	class magnitude_dialog : public dialog
	{
	public:
		magnitude_dialog
			( std::string title
			, std::string prompt
			, double default_value
			, std::optional<double> min
			, std::optional<double> max
			, std::function<void(std::optional<double>)> cont
			)
			: _title{std::move(title)}
			, _prompt{std::move(prompt)}
			, _magnitude{default_value}
			, _min{min}
			, _max{max}
			, _cont{std::move(cont)}
		{
			load_textures();
		}

		state update() final;

		void draw() const final;
	private:
		std::string _title;
		std::string _prompt;
		double _magnitude;
		std::optional<double> _min;
		std::optional<double> _max;
		continuation<std::optional<double>> _cont;

		uptr<sdl::texture> _txt_title;
		uptr<sdl::texture> _txt_prompt;
		uptr<sdl::texture> _txt_selector;

		void load_textures();
	};
}
