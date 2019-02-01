//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "ui/digraph_menu.hpp"

#include <algorithm>

#include "units/spaces::window.hpp"
#include "utility/utility.hpp"

using namespace media;
using namespace units;

namespace ql {
	texture_handle digraph_menu::_ul_handle;
	texture_handle digraph_menu::_ur_handle;
	texture_handle digraph_menu::_dl_handle;
	texture_handle digraph_menu::_dr_handle;
	texture_handle digraph_menu::_u_handle;
	texture_handle digraph_menu::_d_handle;
	texture_handle digraph_menu::_l_handle;
	texture_handle digraph_menu::_r_handle;
	texture_handle digraph_menu::_tile_handle;

	int digraph_menu::_top_margin;
	int digraph_menu::_bottom_margin;
	int digraph_menu::_left_margin;
	int digraph_menu::_right_margin;
	int digraph_menu::_tile_width;
	int digraph_menu::_tile_height;

	initializer<digraph_menu> digraph_menu::_initializer;
	void digraph_menu::initialize() {
		_ul_handle = the_texture_manager().add("resources/textures/menu/ul.png");
		_ur_handle = the_texture_manager().add("resources/textures/menu/ur.png");
		_dl_handle = the_texture_manager().add("resources/textures/menu/dl.png");
		_dr_handle = the_texture_manager().add("resources/textures/menu/dr.png");
		_u_handle = the_texture_manager().add("resources/textures/menu/u.png");
		_d_handle = the_texture_manager().add("resources/textures/menu/d.png");
		_l_handle = the_texture_manager().add("resources/textures/menu/l.png");
		_r_handle = the_texture_manager().add("resources/textures/menu/r.png");
		_tile_handle = the_texture_manager().add("resources/textures/menu/tile.png");
	}

	digraph_menu::digraph_menu(int min_width, int min_height)
		: _content_position{0, 0}
		, _page_index{0}
		, _min_width{min_width}
		, _min_height{min_height}
		, _content_width{0}
		, _content_height{0}
		, _render_is_current{false}
	{}

	void digraph_menu::add_page(std::string title) {
		if (!find(title)) {
			_pages.push_back(page(std::move(title)));
			_render_is_current = false;
		} else {
			throw std::invalid_argument("Page already exists.");
		}
	}

	void digraph_menu::add_option(std::string_view page_title, std::string option_name) {
		std::optional<int> page_index = find(page_title);
		if (page_index) {
			_pages[*page_index].options.push_back(page::option(std::move(option_name), std::nullopt));
			_render_is_current = false;
		} else {
			throw std::invalid_argument("Attempted to add an option to a nonexistent menu page.");
		}
	}

	void digraph_menu::add_option(std::string_view location_page_title, std::string option_name, std::string_view target_page_title) {
		std::optional<int> location_page_index = find(location_page_title);
		if (location_page_index) {
			std::optional<int> target_index = find(target_page_title);
			if (target_index) {
				_pages[location_page_index.value()].options.push_back(page::option(std::move(option_name), target_index.value()));
				_render_is_current = false;
			} else {
				throw std::invalid_argument("Attempted to add an option linking to a nonexistent menu page.");
			}
		} else {
			throw std::invalid_argument("Attempted to add an option to a nonexistent menu page.");
		}
	}

	void digraph_menu::set_page(std::string_view title) {
		for (size_t index = 0; index < _pages.size(); ++index) {
			if (_pages[index].title == title) {
				_page_index = static_cast<int>(index);
				return;
			}
		}
		throw std::invalid_argument("Attempted to navigate to a nonexistent menu page.");
	}
	
	void digraph_menu::set_option(std::string_view page_title, int option_index) {
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

	void digraph_menu::clear() {
		_pages.clear();
		_page_index = 0;
		_render_is_current = false;
	}

	void digraph_menu::update() {
		if (_pages.size() == 0 || !_render_is_current) {
			return;
		}

		static auto hover_sound_handle = the_sound_manager().add("resources/sounds/menu/hover.wav");
		static auto select_sound_handle = the_sound_manager().add("resources/sounds/menu/select.wav");

		// Get index of option over which the mouse is hovering, if any.

		std::optional<int> hovered_option_index = std::nullopt;
		spaces::window::point position = _content_position;
		position.y() += _title_height;
		for (size_t i = 0; i < _pages[_page_index].options.size(); ++i) {
			spaces::window::box box = spaces::window::box
				{ spaces::window::point{position.x(), position.y()}
				, _page_views[_page_index].option_textures[i].size()
				};
			spaces::window::point point = im.mouse_position();
			if (box.contains(point)) {
				hovered_option_index = static_cast<int>(i);
				break;
			}
			position.y() += _option_height;
		}

		// Change option index, if necessary.

		int& current_option_index = _pages[_page_index].option_index;
		if (im.mouse_moved() && hovered_option_index) {
			if (current_option_index != hovered_option_index.value()) {
				the_sound_manager()[hover_sound_handle].play();
				current_option_index = hovered_option_index.value();
			}
		} else {
			if (im.presses(sf::Keyboard::DOWN)) {
				the_sound_manager()[hover_sound_handle].play();
				++current_option_index;
			}
			if (im.presses(sf::Keyboard::UP)) {
				the_sound_manager()[hover_sound_handle].play();
				--current_option_index;
			}
			if (current_option_index < 0) {
				current_option_index = static_cast<int>(_pages[_page_index].options.size() - 1);
			} else if (static_cast<size_t>(current_option_index) >= _pages[_page_index].options.size()) {
				current_option_index = 0;
			}
		}

		// Check for selection.

		if (im.presses(sf::Keyboard::RETURN) || im.presses(sf::Keyboard::SPACE) || hovered_option_index && im.pressed(mouse_button::left)) {
			the_sound_manager()[select_sound_handle].play();
			digraph_menu::page::option selection = _pages[_page_index].options[_pages[_page_index].option_index];
			if (selection.target) {
				_page_index = selection.target.value();
			} else {
				_selections.push_back(make_pair(_pages[_page_index].title, selection.name));
			}
		}
	}

	std::vector<std::pair<std::string, std::string>> digraph_menu::poll_selections() {
		auto selections = _selections;
		_selections.clear();
		return selections;
	}

	void digraph_menu::draw(spaces::window::point origin, spaces::window::h_align horizontal_alignment, spaces::window::v_align vertical_alignment) {
		if (!_render_is_current) {
			render();
		}

		_content_position = origin;
		switch (horizontal_alignment) {
			case spaces::window::align_left:
				break;
			case spaces::window::align_center:
				_content_position.x() -= _content_width / 2;
				break;
			case spaces::window::align_right:
				_content_position.x() -= _content_width;
				break;
		}
		switch (vertical_alignment) {
			case spaces::window::align_top:
				break;
			case spaces::window::align_middle:
				_content_position.y() -= _content_height / 2;
				break;
			case spaces::window::align_bottom:
				_content_position.y() -= _content_height;
				break;
		}

		// Draw background.

		_background->draw(spaces::window::point{_content_position.x() - _left_margin, _content_position.y() - _top_margin});

		// Draw text.

		_page_views[_page_index].title_texture.draw(_content_position);

		constexpr auto unselected_color_vector = colors::black_vector();
		constexpr auto selected_color_vector = colors::red_vector();
		for (int i = 0; i < static_cast<int>(_page_views[_page_index].option_textures.size()); ++i) {
			colors::color_vector option_color_vector = _pages[_page_index].option_index == i ? selected_color_vector : unselected_color_vector;

			spaces::window::point option_position{_content_position.x(), _content_position.y() + _title_height + i * _option_height};
			_page_views[_page_index].option_textures[i].draw(option_position, spaces::window::align_left, spaces::window::align_top, option_color_vector);
		}
	}

	std::optional<int> digraph_menu::find(std::string_view page_title) {
		for (size_t i = 0; i < _pages.size(); ++i) {
			if (_pages[i].title == page_title) {
				return static_cast<int>(i);
			}
		}
		return std::nullopt;
	}

	void digraph_menu::render() {
		constexpr int title_font_size = 48;
		constexpr int option_font_size = 30;

		static auto title_font_handle = the_font_manager().add("resources/fonts/dumbledor1.ttf", title_font_size);
		static auto option_font_handle = the_font_manager().add("resources/fonts/dumbledor1.ttf", option_font_size);

		constexpr auto title_color = colors::black();

		static bool first_call = true;
		if (first_call) {
			// Initialize these on first call to render() rather than at static initialization since they rely on loaded textures.

			_top_margin = the_texture_manager()[_u_handle].height();
			_bottom_margin = the_texture_manager()[_d_handle].height();
			_left_margin = the_texture_manager()[_l_handle].width();
			_right_margin = the_texture_manager()[_r_handle].width();
			_tile_width = the_texture_manager()[_tile_handle].width();
			_tile_height = the_texture_manager()[_tile_handle].height();

			first_call = false;
		}

		_content_width = _min_width - _left_margin - _right_margin;
		_content_height = _min_height - _top_margin - _bottom_margin;

		// Render text.

		_page_views.clear();
		for (size_t i = 0; i < _pages.size(); ++i) {
			_content_height = std::max(_content_height, static_cast<int>(_title_height + _pages[_page_index].options.size() * _option_height));
			std::vector<texture> option_textures;
			for (size_t j = 0; j < _pages[i].options.size(); ++j) {
				option_textures.push_back(the_font_manager()[option_font_handle].render(_pages[i].options[j].name.c_str(), colors::white()));
				_content_width = std::max(_content_width, option_textures[j].width());
			}
			_page_views.emplace_back(the_font_manager()[title_font_handle].render(_pages[i].title.c_str(), title_color), std::move(option_textures));
		}

		int const width_remainder = _content_width % _tile_width;
		if (width_remainder > 0) {
			_content_width += _tile_width - width_remainder;
		}

		int const height_remainder = _content_height % _tile_height;
		if (height_remainder > 0) {
			_content_height += _tile_height - height_remainder;
		}

		// Render background.

		_background = umake<texture>(spaces::window::vector
			{ _content_width + _left_margin + _right_margin
			, _content_height + _top_margin + _bottom_margin
			});
		_background->as_target([&] {
			// Interior
			for (int x = 0; x < _content_width / _tile_width; ++x) {
				for (int y = 0; y < _content_height / _tile_height; ++y) {
					the_texture_manager()[_tile_handle].draw(spaces::window::point{_left_margin + _tile_width * x, _top_margin + _tile_height * y});
				}
			}
			// Top and bottom margins
			for (int x = 0; x < _content_width / _tile_width; ++x) {
				the_texture_manager()[_u_handle].draw(spaces::window::point{_left_margin + _tile_width * x, 0});
				the_texture_manager()[_d_handle].draw(spaces::window::point{_left_margin + _tile_width * x, _top_margin + _content_height});
			}
			// Left and right margins
			for (int y = 0; y < _content_height / _tile_height; ++y) {
				the_texture_manager()[_l_handle].draw(spaces::window::point{0, _top_margin + _tile_width * y});
				the_texture_manager()[_r_handle].draw(spaces::window::point{_left_margin + _content_width, _top_margin + _tile_width * y});
			}
			// Corners
			the_texture_manager()[_ul_handle].draw(spaces::window::point{0, 0});
			the_texture_manager()[_ur_handle].draw(spaces::window::point{_left_margin + _content_width, 0});
			the_texture_manager()[_dl_handle].draw(spaces::window::point{0, _top_margin + _content_height});
			the_texture_manager()[_dr_handle].draw(spaces::window::point{_left_margin + _content_width, _top_margin + _content_height});
		});

		_render_is_current = true;
	}
}
