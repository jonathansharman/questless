/**
* @file    DigraphMenuView.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the DigraphMenuView class.
*/

#include "ui/DigraphMenuView.h"

#include <algorithm>
using std::max;

using std::async;
using std::shared_future;

using namespace::sdl;

namespace questless
{
	shared_future<int> DigraphMenuView::_top_margin;
	shared_future<int> DigraphMenuView::_bottom_margin;
	shared_future<int> DigraphMenuView::_left_margin;
	shared_future<int> DigraphMenuView::_right_margin;
	shared_future<int> DigraphMenuView::_tile_width;
	shared_future<int> DigraphMenuView::_tile_height;
	Initializer<DigraphMenuView> DigraphMenuView::_initializer;
	void DigraphMenuView::initialize()
	{
		font_manager().add("DMV/title", [] { return Font::make("resources/fonts/dumbledor1.ttf", title_font_size, SDL_BLENDMODE_BLEND); });
		font_manager().add("DMV/option", [] { return Font::make("resources/fonts/dumbledor1.ttf", option_font_size, SDL_BLENDMODE_BLEND); });

		texture_manager().add("DMV/ul", [] { return Texture::make("resources/textures/menu/ul.png", renderer(), SDL_BLENDMODE_BLEND); });
		texture_manager().add("DMV/ur", [] { return Texture::make("resources/textures/menu/ur.png", renderer(), SDL_BLENDMODE_BLEND); });
		texture_manager().add("DMV/dl", [] { return Texture::make("resources/textures/menu/dl.png", renderer(), SDL_BLENDMODE_BLEND); });
		texture_manager().add("DMV/dr", [] { return Texture::make("resources/textures/menu/dr.png", renderer(), SDL_BLENDMODE_BLEND); });
		texture_manager().add("DMV/u", [] { return Texture::make("resources/textures/menu/u.png", renderer(), SDL_BLENDMODE_BLEND); });
		texture_manager().add("DMV/d", [] { return Texture::make("resources/textures/menu/d.png", renderer(), SDL_BLENDMODE_BLEND); });
		texture_manager().add("DMV/l", [] { return Texture::make("resources/textures/menu/l.png", renderer(), SDL_BLENDMODE_BLEND); });
		texture_manager().add("DMV/r", [] { return Texture::make("resources/textures/menu/r.png", renderer(), SDL_BLENDMODE_BLEND); });
		texture_manager().add("DMV/tile", [] { return Texture::make("resources/textures/menu/tile.png", renderer(), SDL_BLENDMODE_BLEND); });

		_top_margin = async(std::launch::deferred, [] { return texture_manager()["DMV/u"].height(); });
		_bottom_margin = async(std::launch::deferred, [] { return texture_manager()["DMV/d"].height(); });
		_left_margin = async(std::launch::deferred, [] { return texture_manager()["DMV/l"].width(); });
		_right_margin = async(std::launch::deferred, [] { return texture_manager()["DMV/r"].width(); });
		_tile_width = async(std::launch::deferred, [] { return texture_manager()["DMV/tile"].width(); });
		_tile_height = async(std::launch::deferred, [] { return texture_manager()["DMV/tile"].height(); });
	}

	void DigraphMenuView::render(const DigraphMenuModel& menu)
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
		for (unsigned i = 0; i < menu.pages.size(); ++i) {
			_content_height = max(_content_height, static_cast<int>(title_height + menu.current_options().size() * option_height));
			vector<Texture> option_textures;
			for (unsigned j = 0; j < menu.pages[i].options.size(); ++j) {
				option_textures.push_back(font_manager()["DMV/option"].render(menu.pages[i].options[j].name, renderer(), Color::white()));
				_content_width = max(_content_width, option_textures[j].width());
			}
			_page_views.emplace_back(font_manager()["DMV/title"].render(menu.pages[i].title, renderer(), title_color()), std::move(option_textures));
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

		_background = Texture::make
			( renderer()
			, SDL_BLENDMODE_BLEND
			, _content_width + left_margin + right_margin
			, _content_height + top_margin + bottom_margin
			, true
			);
		_background->as_target([&] {
			// Clear background texture with transparent color.
			renderer().clear(Color::clear());
			
			// Interior
			for (int x = 0; x < _content_width / tile_width; ++x) {
				for (int y = 0; y < _content_height / tile_height; ++y) {
					texture_manager()["DMV/tile"].draw(Point(left_margin + tile_width * x, top_margin + tile_height * y));
				}
			}
			// Top and bottom margins
			for (int x = 0; x < _content_width / tile_width; ++x) {
				texture_manager()["DMV/u"].draw(Point(left_margin + tile_width * x, 0));
				texture_manager()["DMV/d"].draw(Point(left_margin + tile_width * x, top_margin + _content_height));
			}
			// Left and right margins
			for (int y = 0; y < _content_height / tile_height; ++y) {
				texture_manager()["DMV/l"].draw(Point(0, top_margin + tile_width * y));
				texture_manager()["DMV/r"].draw(Point(left_margin + _content_width, top_margin + tile_width * y));
			}
			// Corners
			texture_manager()["DMV/ul"].draw(Point(0, 0));
			texture_manager()["DMV/ur"].draw(Point(left_margin + _content_width, 0));
			texture_manager()["DMV/dl"].draw(Point(0, top_margin + _content_height));
			texture_manager()["DMV/dr"].draw(Point(left_margin + _content_width, top_margin + _content_height));
		});

		_render_is_current = true;
	}

	void DigraphMenuView::update_indices(const DigraphMenuModel& menu)
	{
		_page_index = menu.page_index;
		_current_option_index = menu.current_option_index();
	}

	void DigraphMenuView::position(Point origin, HAlign horizontal_alignment, VAlign vertical_alignment)
	{
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
	}

	void DigraphMenuView::draw() const
	{
		// Draw background.

		_background->draw(Point(_content_position.x - _left_margin.get(), _content_position.y - _top_margin.get()));

		// Draw text.
	
		_page_views[_page_index].title_texture.draw(_content_position);

		for (size_t i = 0; i < _page_views[_page_index].option_textures.size(); ++i) {
			_page_views[_page_index].option_textures[i].color(static_cast<size_t>(_current_option_index) == i ? selected_color() : unselected_color());

			Point option_position(_content_position.x, _content_position.y + title_height + i * option_height);
			_page_views[_page_index].option_textures[i].draw(option_position);
		}
	}
}
