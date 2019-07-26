//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "main_menu.hpp"

#include "playing.hpp"

#include "ui/layout.hpp"

#include "utility/reference.hpp"
#include "world/region.hpp"

namespace ql::scenes {
	using namespace view::literals;

	main_menu::main_menu(sf::RenderWindow& window, rsrc::fonts const& fonts)
		: scene{window, fonts}
		, _layout{window}
		, _main_menu{_layout, fonts, {480.0_px, 640.0_px}} //
	{
		// Add pages.
		_main_menu.add_page("Questless");
		_main_menu.add_page("Settings");
		// Add options.
		_main_menu.add_option("Questless", "Continue");
		_main_menu.add_option("Questless", "Begin Anew");
		_main_menu.add_option("Questless", "Settings", "Settings");
		_main_menu.add_option("Questless", "Being Editor");
		_main_menu.add_option("Questless", "Quit");
		_main_menu.add_option("Settings", "Save", "Questless");
		_main_menu.add_option("Settings", "Cancel", "Questless");
	}

	update_result main_menu::scene_subupdate(sec elapsed_time, std::vector<sf::Event>& events) {
		_main_menu.update(elapsed_time, im);
		for (auto const& [page, option] : _main_menu.poll_selections()) {
			if (page == "Questless") {
				if (option == "Continue" || option == "Begin Anew") {
					ent region_id = make_region(reg.create(), "Region1");

					return switch_scene{umake<playing>(_window, _fonts, region_id)};
				}
			} else if (option == "Being Editor") {
				//! @todo Being Editor scene
			} else if (option == "Quit") {
				return game_over{};
			}
		}
		return continue_scene{};
	}

	void main_menu::scene_subdraw(sf::RenderTarget& target, sf::RenderStates states) const {
		_main_menu.draw(target, states);
	}
}
