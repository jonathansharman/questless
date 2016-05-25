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
	Initializer<Dialog> Dialog::_initializer;
	void Dialog::initialize()
	{
		using namespace sdl;
		font_manager().add("Dialog/title", [] { return Font::make("resources/fonts/dumbledor1.ttf", 32, SDL_BLENDMODE_BLEND); });
		font_manager().add("Dialog/prompt", [] { return Font::make("resources/fonts/dumbledor1.ttf", 20, SDL_BLENDMODE_BLEND); });
		font_manager().add("Dialog/io", [] { return Font::make("resources/fonts/dumbledor1.ttf", 32, SDL_BLENDMODE_BLEND); });
		font_manager().add("Dialog/list-option", [] { return Font::make("resources/fonts/dumbledor1.ttf", 20, SDL_BLENDMODE_BLEND); });
	}
}
