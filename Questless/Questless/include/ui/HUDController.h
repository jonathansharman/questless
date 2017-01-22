/**
* @file    HUDController.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the HUDController, which manages the HUD model and view and handles input to the HUD.
*/

#pragma once

#include "HUDModel.h"
#include "HUDView.h"
#include "sdl-wrappers/Input.h"

namespace questless
{
	class HUDController
	{
	public:
		HUDController(Game& game, const sdl::Window& window);

		/// @return A constant reference to the hotbar.
		const auto& hotbar() { return _hud.hotbar; }

		void update(const sdl::Input& input);

		void HUDController::draw() { _view.draw(_hud); }

		void player_id(BeingId player_id);
	private:
		Game& _game;
		HUDModel _hud;
		HUDView _view;
	};
}
