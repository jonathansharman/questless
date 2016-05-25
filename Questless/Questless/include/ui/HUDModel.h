/**
* @file    HUDModel.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The HUDModel struct, which encapsulates the HUD model data.
*/

#ifndef HUD_MODEL_H
#define HUD_MODEL_H

#include <array>
#include <memory>

#include "entities/beings/Being.h"
#include "items/Inventory.h"
#include "utility/optional.h"

namespace questless
{
	class Being;

	struct HUDModel
	{
		// Constants

		static constexpr size_t hotbar_size = 10;

		// Data

		Being* player_being = nullptr;
		std::array<optional<Inventory::Coords>, hotbar_size> hotbar;

		bool inv_open = false;
		size_t inv_page = 0;

		// Methods

		void update_being_info();
	};
}

#endif