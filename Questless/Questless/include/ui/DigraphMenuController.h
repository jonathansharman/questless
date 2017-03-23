/**
* @file    DigraphMenuController.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the DigraphMenuController class.
*/

#pragma once

#include <utility>
#include <memory>
#include <stdexcept>

#include "utility/utility.h"
#include "DigraphMenuModel.h"
#include "DigraphMenuView.h"
#include "sdl/Input.h"
#include "sdl/resources.h" /// @todo This is used only for sound. Is there a way to move sound into the view? Does it even belong there?
#include "utility/Initializer.h"

namespace questless
{
	class DigraphMenuController
	{
	public:
		/// @param min_width The minimum width of the menu, including margins. If necessary, the menu will stretch to fit its contents.
		/// @param min_height The minimum height of the menu, including margins. If necessary, the menu will stretch to fit its contents.
		DigraphMenuController(int min_width, int min_height);

		/// Adds a blank page to the menu.
		/// @param title The page title.
		/// @note Throws a MenuException if a page with the same title already exists.
		void add_page(std::string const& title);

		/// Adds a terminal option (one which doesn't lead to another page) to the specified page of the menu.
		/// @param page_title The title of the page to which the option is to be added.
		/// @param option_name The name of the option to be added.
		void add_option(std::string const& page_title, std::string const& option_name);

		/// Adds a nonterminal option (one which leads to another page) to the specified page of the menu.
		/// @param location_page_title The title of the page to which the option is to be added.
		/// @param option_name The name of the option to be added.
		/// @param target_page_title The title of the page to which the option links.
		void add_option(std::string const& location_page_title, std::string const& option_name, std::string const& target_page_title);

		/// Navigates to the specified page of the menu.
		/// @param title The title of the page.
		void set_page(std::string const& title);

		/// Selects the specified option on the specified page.
		/// @param page_title The title of the page whose option index is to be set.
		/// @param option_index The index of the option to be selected.
		void set_option(std::string const& page_title, int option_index);

		/// Removes all pages from the menu.
		void clear();
	
		/// @return A constant reference to the options of the current menu page.
		std::vector<DigraphMenuModel::Page::Option> const& current_options() const { return _menu.current_options(); }

		/// @return The index of the selected option on the current menu page.
		int current_option_index() const { return _menu.current_option_index(); }

		/// Updates the menu state. To be called once per frame.
		void update();

		/// Gets all the terminal menu selections since the last call to poll_selections().
		/// @return A vector of pairs of strings, each representing a page name and option name. The pairs are ordered from least to most recently selected.
		std::vector<std::pair<std::string, std::string>> poll_selections();

		/// Draws the menu, rendering it beforehand if necessary.
		/// @param origin The origin point of the menu on the screen.
		/// @param horizontal_alignment The horizontal alignment of the menu relative to the origin point.
		/// @param vertical_alignment The vertical alignment of the menu relative to the origin point.
		////
		void draw(units::ScreenPoint origin, sdl::HAlign horizontal_alignment = sdl::HAlign::left, sdl::VAlign vertical_alignment = sdl::VAlign::top);
	private:
		friend class Initializer<DigraphMenuController>;
		static Initializer<DigraphMenuController> _initializer;
		static void initialize();

		static sdl::SoundHandle _hover_sound_handle, _select_sound_handle;

		DigraphMenuModel _menu;
		DigraphMenuView _view;
		std::vector<std::pair<std::string, std::string>> _selections;

		/// Finds the first page with the given page title, if it exists.
		/// @param page_title The title of the page to be found.
		/// @return The index of the page or nullopt if no matching page was found.
		std::optional<int> find(std::string const& page_title);
	};
}
