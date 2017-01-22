/**
* @file    HUD.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the HUD class.
*/

#include "ui/HUDController.h"
#include "Game.h"
#include "entities/beings/Being.h"

namespace questless
{
	HUDController::HUDController(Game& game, sdl::Window const& window) : _game{game}, _hud {}, _view{game, window}
	{}
	
	void HUDController::update(sdl::Input const& input)
	{
		if (!_hud.player_id) {
			return;
		}
		if (Being* player_being = _game.being(*_hud.player_id)) {
			_hud.update_being_info();

			if (input.presses(SDLK_TAB)) {
				_hud.inv_open = !_hud.inv_open;
			}

			if (_hud.inv_open) {
				int row = (input.y_mouse() - _view.inv_top()) / HUDView::item_icon_height;
				int column = (input.x_mouse() - _view.inv_left()) / HUDView::item_icon_width;
				if (0 <= row && row < Inventory::Page::rows && 0 <= column && column < Inventory::Page::columns) {
					Inventory::Coords coords{_hud.inv_page, static_cast<size_t>(row), static_cast<size_t>(column)};
					if (player_being->inventory()[coords] != nullptr) {
						if (input.presses(SDLK_1)) {
							_hud.hotbar[0] = coords;
						}
						if (input.presses(SDLK_2)) {
							_hud.hotbar[1] = coords;
						}
						if (input.presses(SDLK_3)) {
							_hud.hotbar[2] = coords;
						}
						if (input.presses(SDLK_4)) {
							_hud.hotbar[3] = coords;
						}
						if (input.presses(SDLK_5)) {
							_hud.hotbar[4] = coords;
						}
						if (input.presses(SDLK_6)) {
							_hud.hotbar[5] = coords;
						}
						if (input.presses(SDLK_7)) {
							_hud.hotbar[6] = coords;
						}
						if (input.presses(SDLK_8)) {
							_hud.hotbar[7] = coords;
						}
						if (input.presses(SDLK_9)) {
							_hud.hotbar[8] = coords;
						}
						if (input.presses(SDLK_0)) {
							_hud.hotbar[9] = coords;
						}
					}
				}
			}
		}
	}

	void HUDController::player_id(BeingId player_id)
	{
		_hud.player_id = player_id;
		_hud.inv_page = 0;

		if (Being* player_being = _game.being(player_id)) {
			size_t count = 0;
			for (size_t page = 0; page < player_being->inventory().pages(); ++page) {
				for (size_t row = 0; row < Inventory::Page::rows; ++row) {
					for (size_t column = 0; column < Inventory::Page::columns; ++column) {
						if (player_being->inventory().page(page).items[row][column] != nullptr) {
							_hud.hotbar[count++] = Inventory::Coords{page, row, column};
							if (count == HUDModel::hotbar_size) {
								return;
							}
						}
					}
				}
			}
		} else {
			for (size_t i = 0; i < HUDModel::hotbar_size; ++i) {
				_hud.hotbar[i] = boost::none;
			}
		}
	}
}
