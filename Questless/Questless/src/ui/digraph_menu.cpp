//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "digraph_menu.hpp"

#include "view_space.hpp"

#include "rsrc/fonts.hpp"
#include "utility/utility.hpp"

#include <algorithm>

namespace ql {
	using namespace view::literals;

	namespace {
		constexpr auto title_height = 60.0_px;
		constexpr auto option_height = 32.0_px;
	}

	digraph_menu::digraph_menu(widget& parent, rsrc::fonts const& fonts, view::vector min_size)
		: widget{&parent}
		, fonts{fonts}
		, _min_size{min_size} //
	{
		_top_left_margin = view::vector_from_sfml(resources.txtr.ul.getSize());
		_bottom_right_margin = view::vector_from_sfml(resources.txtr.dr.getSize());
		_tile_size = view::vector_from_sfml(resources.txtr.tile.getSize());
	}

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
				_pages[location_page_index.value()].options.push_back(
					page::option(std::move(option_name), target_index.value()));
				_render_is_current = false;
			} else {
				throw std::invalid_argument("Attempted to add an option linking to a nonexistent menu page.");
			}
		} else {
			throw std::invalid_argument("Attempted to add an option to a nonexistent menu page.");
		}
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

	view::vector digraph_menu::get_local_offset() const {}

	view::vector digraph_menu::get_size() const {}

	void digraph_menu::update(sec elapsed_time, std::vector<sf::Event>& events) {
		if (_pages.size() == 0 || !_render_is_current) { return; }

		// Get index of option over which the mouse is hovering, if any.

		std::optional<int> hovered_option_index = std::nullopt;
		view::point position = _content_position;
		position[1] += title_height;
		for (size_t i = 0; i < _pages[_page_index].options.size(); ++i) {
			view::box box = {position, view::vector_from_sfml(_page_views[_page_index].option_textures[i].getSize())};
			view::point point = im.mouse_position();
			if (box.contains(point)) {
				hovered_option_index = static_cast<int>(i);
				break;
			}
			position[1] += option_height;
		}

		// Change option index, if necessary.

		int& current_option_index = _pages[_page_index].option_index;
		if (im.mouse_moved() && hovered_option_index) {
			if (current_option_index != hovered_option_index.value()) {
				resources.sfx.hover.play();
				current_option_index = hovered_option_index.value();
			}
		} else {
			if (im.pressed(sf::Keyboard::Down)) {
				resources.sfx.hover.play();
				++current_option_index;
			}
			if (im.pressed(sf::Keyboard::Up)) {
				resources.sfx.hover.play();
				--current_option_index;
			}
			if (current_option_index < 0) {
				current_option_index = static_cast<int>(_pages[_page_index].options.size() - 1);
			} else if (static_cast<size_t>(current_option_index) >= _pages[_page_index].options.size()) {
				current_option_index = 0;
			}
		}

		// Check for selection.
		if (im.pressed({sf::Keyboard::Return, sf::Keyboard::Space}) || hovered_option_index && im.pressed(sf::Mouse::Left)) {
			resources.sfx.select.play();
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

	void digraph_menu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		auto const position = get_position();
		_content_position = position + _top_left_margin;

		// Draw background.

		_background->draw(position);

		// Draw text.

		_page_views[_page_index].title_texture.draw(_content_position);

		auto const unselected_color = sf::Color::Black;
		auto const selected_color = sf::Color::Red;
		for (int i = 0; i < static_cast<int>(_page_views[_page_index].option_textures.size()); ++i) {
			auto const option_color = _pages[_page_index].option_index == i ? selected_color : unselected_color;

			view::point option_position{_content_position.x(), _content_position.y() + title_height + i * _option_height};
			_page_views[_page_index].option_textures[i].draw(option_position, option_color);
		}
	}

	std::optional<int> digraph_menu::find(std::string_view page_title) {
		for (size_t i = 0; i < _pages.size(); ++i) {
			if (_pages[i].title == page_title) { return static_cast<int>(i); }
		}
		return std::nullopt;
	}

	void digraph_menu::render() {
		constexpr int option_font_size = 30;

		static auto title_font_handle = the_font_manager().add("resources/fonts/dumbledor1.ttf", title_font_size);
		static auto option_font_handle = the_font_manager().add("resources/fonts/dumbledor1.ttf", option_font_size);

		// Initialize content size to minimum, then expand as necessary.
		_content_size = _min_size - _top_left_margin - _bottom_right_margin;

		// Render text.

		_page_views.clear();
		for (size_t i = 0; i < _pages.size(); ++i) {
			_content_size[1] = std::max(_content_size[1],
				title_height + static_cast<float>(_pages[_page_index].options.size()) * option_height);
			std::vector<sf::Text> option_texts;
			for (size_t j = 0; j < _pages[i].options.size(); ++j) {
				sf::Text option_text{_pages[i].options[j].name, fonts.dumbledor1, 30};
				option_text.setFillColor(sf::Color::White);
				option_texts.push_back(option_text);
				_content_size[0] = std::max(_content_size[0], view::px{option_texts.back().getLocalBounds().width});
			}
			sf::Text title_text{_pages[i].title, fonts.dumbledor1, 48};
			title_text.setFillColor(sf::Color::Black);
			_page_views.emplace_back(title_text, std::move(option_texts));
		}

		int const width_remainder = _content_width % _tile_width;
		if (width_remainder > 0) { _content_width += _tile_width - width_remainder; }

		int const height_remainder = _content_height % _tile_height;
		if (height_remainder > 0) { _content_height += _tile_height - height_remainder; }

		// Render background.

		_background = umake<texture>(spaces::window::vector{
			_content_width + _left_margin + _right_margin, _content_height + _top_margin + _bottom_margin});
		_background->as_target([&] {
			// Interior
			for (int x = 0; x < _content_width / _tile_width; ++x) {
				for (int y = 0; y < _content_height / _tile_height; ++y) {
					the_texture_manager()[_tile_handle].draw(
						spaces::window::point{_left_margin + _tile_width * x, _top_margin + _tile_height * y});
				}
			}
			// Top and bottom margins
			for (int x = 0; x < _content_width / _tile_width; ++x) {
				the_texture_manager()[_u_handle].draw(spaces::window::point{_left_margin + _tile_width * x, 0});
				the_texture_manager()[_d_handle].draw(
					spaces::window::point{_left_margin + _tile_width * x, _top_margin + _content_height});
			}
			// Left and right margins
			for (int y = 0; y < _content_height / _tile_height; ++y) {
				the_texture_manager()[_l_handle].draw(spaces::window::point{0, _top_margin + _tile_width * y});
				the_texture_manager()[_r_handle].draw(
					spaces::window::point{_left_margin + _content_width, _top_margin + _tile_width * y});
			}
			// Corners
			the_texture_manager()[_ul_handle].draw(spaces::window::point{0, 0});
			the_texture_manager()[_ur_handle].draw(spaces::window::point{_left_margin + _content_width, 0});
			the_texture_manager()[_dl_handle].draw(spaces::window::point{0, _top_margin + _content_height});
			the_texture_manager()[_dr_handle].draw(
				spaces::window::point{_left_margin + _content_width, _top_margin + _content_height});
		});

		_render_is_current = true;
	}
}
