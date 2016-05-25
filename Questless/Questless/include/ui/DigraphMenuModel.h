/**
* @file    DigraphMenu.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the DigraphMenuModel class, which represents a directed graph menu system with pages and links.
*/

#ifndef DIGRAPH_MENU_MODEL_H
#define DIGRAPH_MENU_MODEL_H

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <exception>
using std::logic_error;
using std::out_of_range;

#include "utility/constants.h"
#include "utility/optional.h"

namespace questless
{
	struct DigraphMenuModel
	{
		struct Page
		{
			struct Option
			{
				string name;
				optional<int> target;

				/// @param name The name of the option.
				/// @param target The index of page to which this option links, or nullopt if the option is terminal.
				Option(string name, optional<int> target) : name(name), target(target) {}
			};

			string title;
			vector<Option> options;
			int option_index;

			Page(string title) : title(title), option_index(0) {}
		};

		vector<Page> pages;
		unsigned page_index;

		DigraphMenuModel() : page_index(0) {}

		/// @todo Consider refactoring all these reference functions in the menu classes.
		/// @return A reference to the current menu page.
		Page& current_page();
		/// @return A constant reference to the current menu page.
		const Page& current_page() const;

		/// @return A reference to the options of the current menu page.
		vector<Page::Option>& current_options();
		/// @return A constant reference to the options of the current menu page.
		const vector<Page::Option>& current_options() const;

		/// @return A reference to the current option.
		Page::Option& current_option();
		/// @return A constant reference to the current option.
		const Page::Option& current_option() const;

		/// @return A reference to the index of the selected option on the current menu page.
		int& current_option_index();
		/// @return The index of the selected option on the current menu page.
		int current_option_index() const;
	};
}

#endif