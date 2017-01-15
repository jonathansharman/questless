/**
* @file    Dialog.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Dialog abstract base class.
*/

#include "ui/Dialog.h"

#include "sdl-wrappers/Font.h"

namespace questless
{
	sdl::Handle<sdl::Font> Dialog::_title_font_handle;
	sdl::Handle<sdl::Font> Dialog::_prompt_font_handle;
	sdl::Handle<sdl::Font> Dialog::_io_font_handle;
	sdl::Handle<sdl::Font> Dialog::_list_option_font_handle;

	Initializer<Dialog> Dialog::_initializer;
	void Dialog::initialize()
	{
		using namespace sdl;
		_title_font_handle = font_manager().add([] { return std::make_unique<Font>("resources/fonts/dumbledor1.ttf", 32, SDL_BLENDMODE_BLEND); });
		_prompt_font_handle = font_manager().add([] { return std::make_unique<Font>("resources/fonts/dumbledor1.ttf", 20, SDL_BLENDMODE_BLEND); });
		_io_font_handle = font_manager().add([] { return std::make_unique<Font>("resources/fonts/dumbledor1.ttf", 32, SDL_BLENDMODE_BLEND); });
		_list_option_font_handle = font_manager().add([] { return std::make_unique<Font>("resources/fonts/dumbledor1.ttf", 20, SDL_BLENDMODE_BLEND); });
	}
}
