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

#include <boost/optional.hpp>

#include "entities/beings/BeingId.h"
#include "items/Inventory.h"

namespace questless
{
	struct HUDModel
	{
		// Constants

		static constexpr size_t hotbar_size = 10;

		// Data

		boost::optional<BeingId> player_id = boost::none;
		std::array<boost::optional<Inventory::Coords>, hotbar_size> hotbar;

		bool inv_open = false;
		size_t inv_page = 0;

		// Methods

		void update_being_info();
	};
}

#endif
