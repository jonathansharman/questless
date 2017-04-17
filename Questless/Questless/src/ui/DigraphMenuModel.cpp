//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/DigraphMenuModel.h"

using std::logic_error;
using std::out_of_range;

namespace questless
{
	DigraphMenuModel::Page& DigraphMenuModel::current_page()
	{
		if (pages.size() == 0) {
			throw logic_error("Attempted to access current page of an empty menu.");
		} else if (page_index >= 0 && static_cast<size_t>(page_index) < pages.size()) {
			return pages[page_index];
		} else {
			throw out_of_range("Current page index is invalid.");
		}
	}

	DigraphMenuModel::Page const& DigraphMenuModel::current_page() const
	{
		if (pages.size() == 0) {
			throw logic_error("Attempted to access current page of an empty menu.");
		} else if (page_index >= 0 && static_cast<size_t>(page_index) < pages.size()) {
			return pages[page_index];
		} else {
			throw out_of_range("Current page index is invalid.");
		}
	}

	DigraphMenuModel::Page::Option& DigraphMenuModel::current_option()
	{
		return pages[page_index].options[pages[page_index].option_index];
	}
	DigraphMenuModel::Page::Option const& DigraphMenuModel::current_option() const
	{
		return pages[page_index].options[pages[page_index].option_index];
	}

	int& DigraphMenuModel::current_option_index()
	{
		return pages[page_index].option_index;
	}
	int DigraphMenuModel::current_option_index() const
	{
		return pages[page_index].option_index;
	}
}
