//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "main_menu.hpp"

#include "playing.hpp"

#include "utility/reference.hpp"
#include "world/region.hpp"

namespace ql::scenes {
	using namespace view::literals;

	main_menu::main_menu(view::vector window_size, rsrc::fonts const& fonts)
		: scene{fonts}, _window_size{window_size} {}

	update_result main_menu::scene_subupdate(sec /*elapsed_time*/, std::vector<sf::Event>& /*events*/) {
		//! @todo Implement main menu stuff. Currently just immediately switching to playing scene.

		ent region_id = make_region(reg.create(), "Region1");
		return switch_scene{umake<playing>(_window_size, fonts, region_id)};
	}

	void main_menu::scene_subdraw(sf::RenderTarget& /*target*/, sf::RenderStates /*states*/) const {}
}
