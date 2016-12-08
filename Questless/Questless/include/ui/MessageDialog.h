/**
* @file    MessageDialog.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the MessageDialog class, which displays a message to the player.
*/

#ifndef MESSAGE_DIALOG_H
#define MESSAGE_DIALOG_H

#include <string>

#include "Dialog.h"

namespace questless
{
	class MessageDialog : public Dialog
	{
	public:
		MessageDialog(std::string title, std::string prompt, std::function<void()> cont)
			: _title{std::move(title)}, _prompt{std::move(prompt)}, _cont{std::move(cont)}
		{
			load_textures();
		}
		
		bool update(sdl::Input& input) override;

		void draw(const sdl::Window& window) override;
	private:
		std::string _title;
		std::string _prompt;
		Continuation<> _cont;

		sdl::Texture::ptr _txt_title;
		sdl::Texture::ptr _txt_prompt;

		void refresh() override { load_textures(); }

		void load_textures();
	};
}

#endif
