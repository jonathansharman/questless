/**
* @file    DigraphMenuController.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt. @section DESCRIPTION The implementation for the DigraphMenuController class.
*/

#include "ui/DigraphMenuController.h"

using std::string;
using std::vector;
using std::pair;
using std::invalid_argument;

using namespace sdl;
using namespace units;

namespace questless
{
	sdl::Handle<sdl::Sound> DigraphMenuController::_hover_sound_handle, DigraphMenuController::_select_sound_handle;

	Initializer<DigraphMenuController> DigraphMenuController::_initializer;
	void DigraphMenuController::initialize()
	{
		_hover_sound_handle = sound_manager().add([] { return std::make_unique<Sound>("resources/sounds/menu/hover.wav"); });
		_select_sound_handle = sound_manager().add([] { return std::make_unique<Sound>("resources/sounds/menu/select.wav"); });
	}

	DigraphMenuController::DigraphMenuController(int min_width, int min_height) : _view{min_width, min_height} {}

	void DigraphMenuController::add_page(const string& title)
	{
		if (!find(title)) {
			_menu.pages.push_back(DigraphMenuModel::Page(title));
			_view.invalidate_render();
		} else {
			throw invalid_argument("Page already exists.");
		}
	}

	void DigraphMenuController::add_option(const string& page_title, const string& option_name)
	{
		boost::optional<int> page_index = find(page_title);
		if (page_index) {
			_menu.pages[page_index.value()].options.push_back(DigraphMenuModel::Page::Option(option_name, boost::none));
			_view.invalidate_render();
		} else {
			throw invalid_argument("Attempted to add an option to a nonexistent menu page.");
		}
	}

	void DigraphMenuController::add_option(const string& location_page_title, const string& option_name, const string& target_page_title)
	{
		boost::optional<int> location_page_index = find(location_page_title);
		if (location_page_index) {
			boost::optional<int> target_index = find(target_page_title);
			if (target_index) {
				_menu.pages[location_page_index.value()].options.push_back(DigraphMenuModel::Page::Option(option_name, target_index.value()));
				_view.invalidate_render();
			} else {
				throw invalid_argument("Attempted to add an option linking to a nonexistent menu page.");
			}
		} else {
			throw invalid_argument("Attempted to add an option to a nonexistent menu page.");
		}
	}

	void DigraphMenuController::set_page(const string& title)
	{
		for (unsigned index = 0; index < _menu.pages.size(); ++index) {
			if (_menu.pages[index].title == title) {
				_menu.page_index = index;
				return;
			}
		}
		throw invalid_argument("Attempted to navigate to a nonexistent menu page.");
	}
	
	void DigraphMenuController::set_option(const string& page_title, unsigned option_index)
	{
		boost::optional<int> page_index = find(page_title);
		if (!page_index) {
			throw invalid_argument("Attempted to set the option of a nonexistent menu page.");
		} else {
			if (option_index < 0 || option_index >= _menu.pages[page_index.value()].options.size()) {
				throw std::out_of_range("Option index out of bounds.");
			}
			_menu.pages[page_index.value()].option_index = option_index;
		}
	}

	void DigraphMenuController::clear()
	{
		_menu.pages.clear();
		_menu.page_index = 0;
		_view.invalidate_render();
	}

	void DigraphMenuController::update(const Input& input)
	{
		if (_menu.pages.size() == 0 || !_view.render_is_current()) {
			return;
		}

		// Get index of option over which the mouse is hovering, if any.

		boost::optional<int> hovered_option_index = boost::none;
		ScreenPoint position = _view.content_position();
		position.y += DigraphMenuView::title_height;
		for (size_t i = 0; i < current_options().size(); ++i) {
			ScreenVector dimensions = _view.page(_menu.page_index).option_textures[i].dimensions();
			ScreenRect rect = ScreenRect{position.x, position.y, dimensions.x, dimensions.y};
			ScreenPoint point = input.mouse_position();
			if (rect.contains(point)) {
				hovered_option_index = static_cast<int>(i);
				break;
			}
			position.y += DigraphMenuView::option_height;
		}

		// Change option index, if necessary.

		if (input.mouse_moved() && hovered_option_index) {
			if (_menu.current_option_index() != hovered_option_index.value()) {
				sound_manager()[_hover_sound_handle].play();
				_menu.current_option_index() = hovered_option_index.value();
			}
		} else {
			if (input.presses(SDLK_DOWN)) {
				sound_manager()[_hover_sound_handle].play();
				++_menu.current_option_index();
			}
			if (input.presses(SDLK_UP)) {
				sound_manager()[_hover_sound_handle].play();
				--_menu.current_option_index();
			}
			if (_menu.current_option_index() < 0) {
				_menu.current_option_index() = _menu.current_options().size() - 1;
			} else if (static_cast<unsigned>(_menu.current_option_index()) >= _menu.current_options().size()) {
				_menu.current_option_index() = 0;
			}
		}

		// Check for selection.

		if (input.presses(SDLK_RETURN) || input.presses(SDLK_SPACE) || hovered_option_index && input.pressed(MouseButton::left)) {
			sound_manager()[_select_sound_handle].play();
			DigraphMenuModel::Page::Option selection = _menu.current_option();
			if (selection.target) {
				_menu.page_index = selection.target.value();
			} else {
				_selections.push_back(make_pair(_menu.current_page().title, selection.name));
			}
		}

		_view.update_indices(_menu);
	}

	vector<pair<string, string>> DigraphMenuController::poll_selections()
	{
		auto selections = _selections;
		_selections.clear();
		return selections;
	}

	void DigraphMenuController::draw(ScreenPoint origin, HAlign horizontal_alignment, VAlign vertical_alignment)
	{
		if (!_view.render_is_current()) {
			_view.render(_menu);
		}
		_view.position(origin, horizontal_alignment, vertical_alignment);
		_view.draw();
	}

	boost::optional<int> DigraphMenuController::find(const string& page_title)
	{
		for (unsigned i = 0; i < _menu.pages.size(); ++i) {
			if (_menu.pages[i].title == page_title) {
				return i;
			}
		}
		return boost::none;
	}
}
