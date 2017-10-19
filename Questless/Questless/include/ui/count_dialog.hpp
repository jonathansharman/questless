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
	//! Retrieves a count from the player.
	class count_dialog : public dialog
	{
	public:
		count_dialog
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

		state update() final;

		void draw() const final;
	private:
		std::string _title;
		std::string _prompt;
		int _count;
		std::optional<int> _min;
		std::optional<int> _max;
		continuation<std::optional<int>> _cont;

		uptr<sdl::texture> _txt_title;
		uptr<sdl::texture> _txt_prompt;
		uptr<sdl::texture> _txt_selector;

		void refresh() final { load_textures(); }

		void load_textures();
	};
}
