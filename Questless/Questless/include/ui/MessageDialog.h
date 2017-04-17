//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>

#include "Dialog.h"

namespace questless
{
	//! Displays a message to the player.
	class MessageDialog : public Dialog
	{
	public:
		MessageDialog(std::string title, std::string prompt, std::function<void()> cont)
			: _title{std::move(title)}, _prompt{std::move(prompt)}, _cont{std::move(cont)}
		{
			load_textures();
		}
		
		State update() override;

		void draw() const override;
	private:
		std::string _title;
		std::string _prompt;
		Continuation<> _cont;

		sdl::Texture::uptr _txt_title;
		sdl::Texture::uptr _txt_prompt;

		void refresh() override { load_textures(); }

		void load_textures();
	};
}
