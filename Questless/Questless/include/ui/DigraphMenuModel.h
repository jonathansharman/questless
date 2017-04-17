//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <vector>
#include <optional>

namespace questless
{
	struct DigraphMenuModel
	{
		struct Page
		{
			struct Option
			{
				std::string name;
				std::optional<int> target;

				//! @param name The name of the option.
				//! @param target The index of page to which this option links, or nullopt if the option is terminal.
				Option(std::string name, std::optional<int> target) : name{std::move(name)}, target{target} {}
			};

			std::string title;
			std::vector<Option> options;
			int option_index;

			Page(std::string title) : title{std::move(title)}, option_index{0} {}
		};

		std::vector<Page> pages;
		int page_index;

		DigraphMenuModel() : page_index{0} {}

		//! @todo Consider refactoring all these reference functions in the menu classes.
		//! A reference to the current menu page.
		Page& current_page();
		//! A constant reference to the current menu page.
		Page const& current_page() const;

		//! A reference to the options of the current menu page.
		std::vector<Page::Option>& current_options() { return pages[page_index].options; }
		//! A constant reference to the options of the current menu page.
		std::vector<Page::Option> const& current_options() const { return pages[page_index].options; }

		//! A reference to the current option.
		Page::Option& current_option();
		//! A constant reference to the current option.
		Page::Option const& current_option() const;

		//! A reference to the index of the selected option on the current menu page.
		int& current_option_index();
		//! The index of the selected option on the current menu page.
		int current_option_index() const;
	};
}
