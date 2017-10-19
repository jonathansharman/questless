//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>

#include "dialog.hpp"

namespace ql
{
	//! Displays a message to the player.
	class message_dialog : public dialog
	{
	public:
		message_dialog(std::string title, std::string prompt, std::function<void()> cont)
			: _title{std::move(title)}, _prompt{std::move(prompt)}, _cont{std::move(cont)}
		{
			load_textures();
		}
		
		state update() final;

		void draw() const final;
	private:
		std::string _title;
		std::string _prompt;
		continuation<> _cont;

		uptr<sdl::texture> _txt_title;
		uptr<sdl::texture> _txt_prompt;

		void refresh() final { load_textures(); }

		void load_textures();
	};
}
