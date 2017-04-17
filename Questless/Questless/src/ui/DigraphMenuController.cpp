//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/DigraphMenuController.h"

#include "units/ScreenVector.h"
#include "units/ScreenRect.h"

using std::string;
using std::string_view;
using std::vector;
using std::pair;
using std::invalid_argument;

using namespace units;
using namespace sdl;

namespace questless
{
	sdl::SoundHandle DigraphMenuController::_hover_sound_handle;
	sdl::SoundHandle DigraphMenuController::_select_sound_handle;

	Initializer<DigraphMenuController> DigraphMenuController::_initializer;
	void DigraphMenuController::initialize()
	{
		_hover_sound_handle = sound_manager().add("resources/sounds/menu/hover.wav");
		_select_sound_handle = sound_manager().add("resources/sounds/menu/select.wav");
	}

	DigraphMenuController::DigraphMenuController(int min_width, int min_height) : _view{min_width, min_height} {}

	void DigraphMenuController::add_page(string title)
	{
		if (!find(title)) {
			_menu.pages.push_back(DigraphMenuModel::Page(std::move(title)));
			_view.invalidate_render();
		} else {
			throw invalid_argument("Page already exists.");
		}
	}

	void DigraphMenuController::add_option(string_view page_title, string option_name)
	{
		std::optional<int> page_index = find(page_title);
		if (page_index) {
			_menu.pages[page_index.value()].options.push_back(DigraphMenuModel::Page::Option(std::move(option_name), std::nullopt));
			_view.invalidate_render();
		} else {
			throw invalid_argument("Attempted to add an option to a nonexistent menu page.");
		}
	}

	void DigraphMenuController::add_option(string_view location_page_title, string option_name, string_view target_page_title)
	{
		std::optional<int> location_page_index = find(location_page_title);
		if (location_page_index) {
			std::optional<int> target_index = find(target_page_title);
			if (target_index) {
				_menu.pages[location_page_index.value()].options.push_back(DigraphMenuModel::Page::Option(std::move(option_name), target_index.value()));
				_view.invalidate_render();
			} else {
				throw invalid_argument("Attempted to add an option linking to a nonexistent menu page.");
			}
		} else {
			throw invalid_argument("Attempted to add an option to a nonexistent menu page.");
		}
	}

	void DigraphMenuController::set_page(string_view title)
	{
		for (size_t index = 0; index < _menu.pages.size(); ++index) {
			if (_menu.pages[index].title == title) {
				_menu.page_index = static_cast<int>(index);
				return;
			}
		}
		throw invalid_argument("Attempted to navigate to a nonexistent menu page.");
	}
	
	void DigraphMenuController::set_option(string_view page_title, int option_index)
	{
		std::optional<int> page_index = find(page_title);
		if (!page_index) {
			throw invalid_argument("Attempted to set the option of a nonexistent menu page.");
		} else {
			if (option_index < 0 || option_index >= static_cast<int>(_menu.pages[page_index.value()].options.size())) {
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

	void DigraphMenuController::update()
	{
		if (_menu.pages.size() == 0 || !_view.render_is_current()) {
			return;
		}

		// Get index of option over which the mouse is hovering, if any.

		std::optional<int> hovered_option_index = std::nullopt;
		ScreenPoint position = _view.content_position();
		position.y += DigraphMenuView::title_height;
		for (size_t i = 0; i < current_options().size(); ++i) {
			ScreenVector dimensions = _view.page(_menu.page_index).option_textures[i].dimensions();
			ScreenRect rect = ScreenRect{position.x, position.y, dimensions.x, dimensions.y};
			ScreenPoint point = input().mouse_position();
			if (rect.contains(point)) {
				hovered_option_index = static_cast<int>(i);
				break;
			}
			position.y += DigraphMenuView::option_height;
		}

		// Change option index, if necessary.

		if (input().mouse_moved() && hovered_option_index) {
			if (_menu.current_option_index() != hovered_option_index.value()) {
				sound_manager()[_hover_sound_handle].play();
				_menu.current_option_index() = hovered_option_index.value();
			}
		} else {
			if (input().presses(SDLK_DOWN)) {
				sound_manager()[_hover_sound_handle].play();
				++_menu.current_option_index();
			}
			if (input().presses(SDLK_UP)) {
				sound_manager()[_hover_sound_handle].play();
				--_menu.current_option_index();
			}
			if (_menu.current_option_index() < 0) {
				_menu.current_option_index() = static_cast<int>(_menu.current_options().size() - 1);
			} else if (static_cast<size_t>(_menu.current_option_index()) >= _menu.current_options().size()) {
				_menu.current_option_index() = 0;
			}
		}

		// Check for selection.

		if (input().presses(SDLK_RETURN) || input().presses(SDLK_SPACE) || hovered_option_index && input().pressed(MouseButton::left)) {
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

	std::optional<int> DigraphMenuController::find(string_view page_title)
	{
		for (size_t i = 0; i < _menu.pages.size(); ++i) {
			if (_menu.pages[i].title == page_title) {
				return static_cast<int>(i);
			}
		}
		return std::nullopt;
	}
}
