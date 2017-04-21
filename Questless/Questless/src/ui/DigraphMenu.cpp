//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/DigraphMenu.h"

#include <algorithm>

#include "units/ScreenRect.h"
#include "units/ScreenVector.h"

using namespace sdl;
using namespace units;

namespace questless
{
	sdl::SoundHandle DigraphMenu::_hover_sound_handle;
	sdl::SoundHandle DigraphMenu::_select_sound_handle;

	FontHandle DigraphMenu::_title_font_handle;
	FontHandle DigraphMenu::_option_font_handle;

	TextureHandle DigraphMenu::_ul_handle;
	TextureHandle DigraphMenu::_ur_handle;
	TextureHandle DigraphMenu::_dl_handle;
	TextureHandle DigraphMenu::_dr_handle;
	TextureHandle DigraphMenu::_u_handle;
	TextureHandle DigraphMenu::_d_handle;
	TextureHandle DigraphMenu::_l_handle;
	TextureHandle DigraphMenu::_r_handle;
	TextureHandle DigraphMenu::_tile_handle;

	std::shared_future<int> DigraphMenu::_top_margin;
	std::shared_future<int> DigraphMenu::_bottom_margin;
	std::shared_future<int> DigraphMenu::_left_margin;
	std::shared_future<int> DigraphMenu::_right_margin;
	std::shared_future<int> DigraphMenu::_tile_width;
	std::shared_future<int> DigraphMenu::_tile_height;

	Initializer<DigraphMenu> DigraphMenu::_initializer;
	void DigraphMenu::initialize()
	{
		_hover_sound_handle = sound_manager().add("resources/sounds/menu/hover.wav");
		_select_sound_handle = sound_manager().add("resources/sounds/menu/select.wav");

		_title_font_handle = font_manager().add("resources/fonts/dumbledor1.ttf", title_font_size);
		_option_font_handle = font_manager().add("resources/fonts/dumbledor1.ttf", option_font_size);

		_ul_handle = texture_manager().add("resources/textures/menu/ul.png");
		_ur_handle = texture_manager().add("resources/textures/menu/ur.png");
		_dl_handle = texture_manager().add("resources/textures/menu/dl.png");
		_dr_handle = texture_manager().add("resources/textures/menu/dr.png");
		_u_handle = texture_manager().add("resources/textures/menu/u.png");
		_d_handle = texture_manager().add("resources/textures/menu/d.png");
		_l_handle = texture_manager().add("resources/textures/menu/l.png");
		_r_handle = texture_manager().add("resources/textures/menu/r.png");
		_tile_handle = texture_manager().add("resources/textures/menu/tile.png");

		_top_margin = std::async(std::launch::deferred, []() { return texture_manager()[_u_handle].height(); });
		_bottom_margin = std::async(std::launch::deferred, []() { return texture_manager()[_d_handle].height(); });
		_left_margin = std::async(std::launch::deferred, []() { return texture_manager()[_l_handle].width(); });
		_right_margin = std::async(std::launch::deferred, []() { return texture_manager()[_r_handle].width(); });
		_tile_width = std::async(std::launch::deferred, []() { return texture_manager()[_tile_handle].width(); });
		_tile_height = std::async(std::launch::deferred, []() { return texture_manager()[_tile_handle].height(); });
	}

	DigraphMenu::DigraphMenu(int min_width, int min_height)
		: _content_position{0, 0}
		, _page_index{0}
		, _min_width{min_width}
		, _min_height{min_height}
		, _content_width{0}
		, _content_height{0}
		, _render_is_current{false}
	{}

	void DigraphMenu::add_page(std::string title)
	{
		if (!find(title)) {
			_pages.push_back(Page(std::move(title)));
			_render_is_current = false;
		} else {
			throw std::invalid_argument("Page already exists.");
		}
	}

	void DigraphMenu::add_option(std::string_view page_title, std::string option_name)
	{
		std::optional<int> page_index = find(page_title);
		if (page_index) {
			_pages[*page_index].options.push_back(Page::Option(std::move(option_name), std::nullopt));
			_render_is_current = false;
		} else {
			throw std::invalid_argument("Attempted to add an option to a nonexistent menu page.");
		}
	}

	void DigraphMenu::add_option(std::string_view location_page_title, std::string option_name, std::string_view target_page_title)
	{
		std::optional<int> location_page_index = find(location_page_title);
		if (location_page_index) {
			std::optional<int> target_index = find(target_page_title);
			if (target_index) {
				_pages[location_page_index.value()].options.push_back(Page::Option(std::move(option_name), target_index.value()));
				_render_is_current = false;
			} else {
				throw std::invalid_argument("Attempted to add an option linking to a nonexistent menu page.");
			}
		} else {
			throw std::invalid_argument("Attempted to add an option to a nonexistent menu page.");
		}
	}

	void DigraphMenu::set_page(std::string_view title)
	{
		for (size_t index = 0; index < _pages.size(); ++index) {
			if (_pages[index].title == title) {
				_page_index = static_cast<int>(index);
				return;
			}
		}
		throw std::invalid_argument("Attempted to navigate to a nonexistent menu page.");
	}
	
	void DigraphMenu::set_option(std::string_view page_title, int option_index)
	{
		std::optional<int> page_index = find(page_title);
		if (!page_index) {
			throw std::invalid_argument("Attempted to set the option of a nonexistent menu page.");
		} else {
			if (option_index < 0 || option_index >= static_cast<int>(_pages[page_index.value()].options.size())) {
				throw std::out_of_range("Option index out of bounds.");
			}
			_pages[page_index.value()].option_index = option_index;
		}
	}

	void DigraphMenu::clear()
	{
		_pages.clear();
		_page_index = 0;
		_render_is_current = false;
	}

	void DigraphMenu::update()
	{
		if (_pages.size() == 0 || !_render_is_current) {
			return;
		}

		// Get index of option over which the mouse is hovering, if any.

		std::optional<int> hovered_option_index = std::nullopt;
		ScreenPoint position = _content_position;
		position.y += title_height;
		for (size_t i = 0; i < _pages[_page_index].options.size(); ++i) {
			ScreenVector dimensions = _page_views[_page_index].option_textures[i].dimensions();
			ScreenRect rect = ScreenRect{position.x, position.y, dimensions.x, dimensions.y};
			ScreenPoint point = input().mouse_position();
			if (rect.contains(point)) {
				hovered_option_index = static_cast<int>(i);
				break;
			}
			position.y += option_height;
		}

		// Change option index, if necessary.

		int& current_option_index = _pages[_page_index].option_index;
		if (input().mouse_moved() && hovered_option_index) {
			if (current_option_index != hovered_option_index.value()) {
				sound_manager()[_hover_sound_handle].play();
				current_option_index = hovered_option_index.value();
			}
		} else {
			if (input().presses(SDLK_DOWN)) {
				sound_manager()[_hover_sound_handle].play();
				++current_option_index;
			}
			if (input().presses(SDLK_UP)) {
				sound_manager()[_hover_sound_handle].play();
				--current_option_index;
			}
			if (current_option_index < 0) {
				current_option_index = static_cast<int>(_pages[_page_index].options.size() - 1);
			} else if (static_cast<size_t>(current_option_index) >= _pages[_page_index].options.size()) {
				current_option_index = 0;
			}
		}

		// Check for selection.

		if (input().presses(SDLK_RETURN) || input().presses(SDLK_SPACE) || hovered_option_index && input().pressed(MouseButton::left)) {
			sound_manager()[_select_sound_handle].play();
			DigraphMenu::Page::Option selection = _pages[_page_index].options[_pages[_page_index].option_index];
			if (selection.target) {
				_page_index = selection.target.value();
			} else {
				_selections.push_back(make_pair(_pages[_page_index].title, selection.name));
			}
		}
	}

	std::vector<std::pair<std::string, std::string>> DigraphMenu::poll_selections()
	{
		auto selections = _selections;
		_selections.clear();
		return selections;
	}

	void DigraphMenu::draw(ScreenPoint origin, HAlign horizontal_alignment, VAlign vertical_alignment)
	{
		if (!_render_is_current) {
			render();
		}

		_content_position = origin;
		switch (horizontal_alignment) {
		case HAlign::left:
			break;
		case HAlign::center:
			_content_position.x -= _content_width / 2;
			break;
		case HAlign::right:
			_content_position.x -= _content_width;
			break;
		}
		switch (vertical_alignment) {
		case VAlign::top:
			break;
		case VAlign::middle:
			_content_position.y -= _content_height / 2;
			break;
		case VAlign::bottom:
			_content_position.y -= _content_height;
			break;
		}

		// Draw background.

		_background->draw(ScreenPoint{_content_position.x - _left_margin.get(), _content_position.y - _top_margin.get()});

		// Draw text.

		_page_views[_page_index].title_texture.draw(_content_position);

		for (int i = 0; i < static_cast<int>(_page_views[_page_index].option_textures.size()); ++i) {
			Color option_color = _pages[_page_index].option_index == i ? _selected_color : _unselected_color;
			_page_views[_page_index].option_textures[i].color(option_color);

			ScreenPoint option_position{_content_position.x, _content_position.y + title_height + i * option_height};
			_page_views[_page_index].option_textures[i].draw(option_position);
		}
	}

	std::optional<int> DigraphMenu::find(std::string_view page_title)
	{
		for (size_t i = 0; i < _pages.size(); ++i) {
			if (_pages[i].title == page_title) {
				return static_cast<int>(i);
			}
		}
		return std::nullopt;
	}

	void DigraphMenu::render()
	{
		static int top_margin = _top_margin.get();
		static int bottom_margin = _bottom_margin.get();
		static int left_margin = _left_margin.get();
		static int right_margin = _right_margin.get();
		static int tile_width = _tile_width.get();
		static int tile_height = _tile_height.get();

		_content_width = _min_width - left_margin - right_margin;
		_content_height = _min_height - top_margin - bottom_margin;

		// Render text.

		_page_views.clear();
		for (size_t i = 0; i < _pages.size(); ++i) {
			_content_height = std::max(_content_height, static_cast<int>(title_height + _pages[_page_index].options.size() * option_height));
			std::vector<Texture> option_textures;
			for (size_t j = 0; j < _pages[i].options.size(); ++j) {
				option_textures.push_back(font_manager()[_option_font_handle].render(_pages[i].options[j].name.c_str(), Color::white()));
				_content_width = std::max(_content_width, option_textures[j].width());
			}
			_page_views.emplace_back(font_manager()[_title_font_handle].render(_pages[i].title.c_str(), _title_color), std::move(option_textures));
		}

		int width_remainder = _content_width % tile_width;
		if (width_remainder > 0) {
			_content_width += tile_width - width_remainder;
		}

		int height_remainder = _content_height % tile_height;
		if (height_remainder > 0) {
			_content_height += tile_height - height_remainder;
		}

		// Render background.

		_background = std::make_unique<Texture>
			( _content_width + left_margin + right_margin
			, _content_height + top_margin + bottom_margin
			);
		_background->as_target([&] {
			// Clear background texture with transparent color.
			renderer().clear(Color::clear());
			
			// Interior
			for (int x = 0; x < _content_width / tile_width; ++x) {
				for (int y = 0; y < _content_height / tile_height; ++y) {
					texture_manager()[_tile_handle].draw(ScreenPoint{left_margin + tile_width * x, top_margin + tile_height * y});
				}
			}
			// Top and bottom margins
			for (int x = 0; x < _content_width / tile_width; ++x) {
				texture_manager()[_u_handle].draw(ScreenPoint{left_margin + tile_width * x, 0});
				texture_manager()[_d_handle].draw(ScreenPoint{left_margin + tile_width * x, top_margin + _content_height});
			}
			// Left and right margins
			for (int y = 0; y < _content_height / tile_height; ++y) {
				texture_manager()[_l_handle].draw(ScreenPoint{0, top_margin + tile_width * y});
				texture_manager()[_r_handle].draw(ScreenPoint{left_margin + _content_width, top_margin + tile_width * y});
			}
			// Corners
			texture_manager()[_ul_handle].draw(ScreenPoint{0, 0});
			texture_manager()[_ur_handle].draw(ScreenPoint{left_margin + _content_width, 0});
			texture_manager()[_dl_handle].draw(ScreenPoint{0, top_margin + _content_height});
			texture_manager()[_dr_handle].draw(ScreenPoint{left_margin + _content_width, top_margin + _content_height});
		});

		_render_is_current = true;
	}
}
