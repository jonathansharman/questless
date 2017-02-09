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

using namespace sdl;
using namespace units;

namespace questless
{
	Handle<Font> DigraphMenuView::_title_font_handle;
	Handle<Font> DigraphMenuView::_option_font_handle;

	Handle<Texture> DigraphMenuView::_ul_handle;
	Handle<Texture> DigraphMenuView::_ur_handle;
	Handle<Texture> DigraphMenuView::_dl_handle;
	Handle<Texture> DigraphMenuView::_dr_handle;
	Handle<Texture> DigraphMenuView::_u_handle;
	Handle<Texture> DigraphMenuView::_d_handle;
	Handle<Texture> DigraphMenuView::_l_handle;
	Handle<Texture> DigraphMenuView::_r_handle;
	Handle<Texture> DigraphMenuView::_tile_handle;

	shared_future<int> DigraphMenuView::_top_margin;
	shared_future<int> DigraphMenuView::_bottom_margin;
	shared_future<int> DigraphMenuView::_left_margin;
	shared_future<int> DigraphMenuView::_right_margin;
	shared_future<int> DigraphMenuView::_tile_width;
	shared_future<int> DigraphMenuView::_tile_height;

	Initializer<DigraphMenuView> DigraphMenuView::_initializer;
	void DigraphMenuView::initialize()
	{
		_title_font_handle = font_manager().add([] { return std::make_unique<Font>("resources/fonts/dumbledor1.ttf", title_font_size, SDL_BLENDMODE_BLEND); });
		_option_font_handle = font_manager().add([] { return std::make_unique<Font>("resources/fonts/dumbledor1.ttf", option_font_size, SDL_BLENDMODE_BLEND); });

		_ul_handle = texture_manager().add([] { return std::make_unique<Texture>("resources/textures/menu/ul.png", renderer(), SDL_BLENDMODE_BLEND); });
		_ur_handle = texture_manager().add([] { return std::make_unique<Texture>("resources/textures/menu/ur.png", renderer(), SDL_BLENDMODE_BLEND); });
		_dl_handle = texture_manager().add([] { return std::make_unique<Texture>("resources/textures/menu/dl.png", renderer(), SDL_BLENDMODE_BLEND); });
		_dr_handle = texture_manager().add([] { return std::make_unique<Texture>("resources/textures/menu/dr.png", renderer(), SDL_BLENDMODE_BLEND); });
		_u_handle = texture_manager().add([] { return std::make_unique<Texture>("resources/textures/menu/u.png", renderer(), SDL_BLENDMODE_BLEND); });
		_d_handle = texture_manager().add([] { return std::make_unique<Texture>("resources/textures/menu/d.png", renderer(), SDL_BLENDMODE_BLEND); });
		_l_handle = texture_manager().add([] { return std::make_unique<Texture>("resources/textures/menu/l.png", renderer(), SDL_BLENDMODE_BLEND); });
		_r_handle = texture_manager().add([] { return std::make_unique<Texture>("resources/textures/menu/r.png", renderer(), SDL_BLENDMODE_BLEND); });
		_tile_handle = texture_manager().add([] { return std::make_unique<Texture>("resources/textures/menu/tile.png", renderer(), SDL_BLENDMODE_BLEND); });

		_top_margin = async(std::launch::deferred, [&] { return texture_manager()[_u_handle].height(); });
		_bottom_margin = async(std::launch::deferred, [&] { return texture_manager()[_d_handle].height(); });
		_left_margin = async(std::launch::deferred, [&] { return texture_manager()[_l_handle].width(); });
		_right_margin = async(std::launch::deferred, [&] { return texture_manager()[_r_handle].width(); });
		_tile_width = async(std::launch::deferred, [&] { return texture_manager()[_tile_handle].width(); });
		_tile_height = async(std::launch::deferred, [&] { return texture_manager()[_tile_handle].height(); });
	}

	void DigraphMenuView::render(DigraphMenuModel const& menu)
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
			std::vector<Texture> option_textures;
			for (unsigned j = 0; j < menu.pages[i].options.size(); ++j) {
				option_textures.push_back(font_manager()[_option_font_handle].render(menu.pages[i].options[j].name, renderer(), Color::white()));
				_content_width = max(_content_width, option_textures[j].width());
			}
			_page_views.emplace_back(font_manager()[_title_font_handle].render(menu.pages[i].title, renderer(), title_color()), std::move(option_textures));
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

	void DigraphMenuView::update_indices(DigraphMenuModel const& menu)
	{
		_page_index = menu.page_index;
		_current_option_index = menu.current_option_index();
	}

	void DigraphMenuView::position(ScreenPoint origin, HAlign horizontal_alignment, VAlign vertical_alignment)
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

		_background->draw(position());

		// Draw text.
	
		_page_views[_page_index].title_texture.draw(_content_position);

		for (size_t i = 0; i < _page_views[_page_index].option_textures.size(); ++i) {
			_page_views[_page_index].option_textures[i].color(static_cast<size_t>(_current_option_index) == i ? selected_color() : unselected_color());

			ScreenPoint option_position{_content_position.x, _content_position.y + title_height + static_cast<int>(i) * option_height};
			_page_views[_page_index].option_textures[i].draw(option_position);
		}
	}
}
