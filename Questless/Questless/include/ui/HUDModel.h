/**
* @file    HUDModel.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The HUDModel struct, which encapsulates the HUD model data.
*/

#pragma once

#include <array>

#include <boost/optional.hpp>

#include "items/Inventory.h"
#include "utility/Id.h"

namespace questless
{
	struct HUDModel
	{
		static constexpr size_t hotbar_size = 10;

		boost::optional<Id<Being>> player_id = boost::none;
		std::array<boost::optional<Inventory::Coords>, hotbar_size> hotbar;
		bool inv_open = false;
		int inv_page = 0;

		void update_being_info();
	};
}
