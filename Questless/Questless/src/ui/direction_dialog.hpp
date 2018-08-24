//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <optional>

#include "dialog.hpp"
#include "world/coordinates.hpp"

namespace ql {
	//! Retrieves a direction from the player.
	class direction_dialog : public dialog {
	public:
		direction_dialog
			( std::string title
			, std::string prompt
			, std::function<void(std::optional<region_tile::direction>)> cont
			)
			: _title{std::move(title)}
			, _prompt{std::move(prompt)}
			, _cont{std::move(cont)}
		{
			load_textures();
		}

		state update() final;

		void draw() const final;
	private:
		std::string _title;
		std::string _prompt;
		continuation<std::optional<region_tile::direction>> _cont;

		uptr<sdl::texture> _txt_title;
		uptr<sdl::texture> _txt_prompt;

		void load_textures();

	};
}
