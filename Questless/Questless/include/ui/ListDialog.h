/**
* @file    ListDialog.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the ListDialog class, which allows the player to choose from a list of options.
*/

#pragma once

#include <string>
#include <vector>

#include "Dialog.h"
#include "units/ScreenRect.h"
#include "sdl/resources.h"

namespace questless
{
	class ListDialog : public Dialog
	{
	public:
		ListDialog
			( units::ScreenPoint origin
			, std::string title
			, std::vector<std::string> options
			, std::function<void(boost::optional<int>)> cont
			)
			: _bounds{origin.x, origin.y, 0, 0}
			, _title{std::move(title)}
			, _options{std::move(options)}
			, _cont{std::move(cont)}
			, _selection{0}
		{
			load_textures();
		}

		State update() override
		{
			if (sdl::input().presses(SDLK_BACKSPACE) || sdl::input().presses(SDLK_ESCAPE)) {
				return _cont(boost::none);
			}

			int const option_count = static_cast<int>(_options.size());

			if (!_options.empty()) {
				_selection -= sdl::input().presses(SDLK_UP);
				_selection += sdl::input().presses(SDLK_DOWN);
				_selection = _selection % option_count;
				_selection = _selection < 0 ? _selection + option_count : _selection;

				int const end = option_count <= 10 ? option_count : 10;
				for (int i = 0; i < end; ++i) {
					if (sdl::input().presses(sdl::Input::index_to_num_key(i))) {
						if (_selection == i) {
							return _cont(_selection);
						} else {
							_selection = i;
							break;
						}
					}
				}

				if (sdl::input().pressed(sdl::MouseButton::left) || sdl::input().presses(SDLK_RETURN) || sdl::input().presses(SDLK_SPACE)) {
					return _cont(_selection);
				}
			}

			return State::open;
		}

		void draw() const override
		{
			// Draw background.
			sdl::renderer().draw_rect(_bounds, sdl::Color::black(), sdl::Color{255, 200, 150});

			// Draw highlight.
			sdl::renderer().draw_rect(units::ScreenRect{_bounds.x + _x_padding, _bounds.y + _y_padding + _title_height + _selection * _option_height, _bounds.w - 2 * _x_padding, _option_height}, sdl::Color::white(), true);

			// Draw title.
			_txt_title->draw(units::ScreenPoint{_bounds.x + _x_padding, _bounds.y + _y_padding});

			// Draw options.
			for (size_t i = 0; i < _options.size(); ++i) {
				int y = _bounds.y + _y_padding + _title_height + static_cast<int>(i) * _option_height;
				_txt_options[i]->draw(units::ScreenPoint{_bounds.x + _x_padding, y});
			}
		}
	private:
		static constexpr int _title_height = 40;
		static constexpr int _option_height = 20;
		static constexpr int _x_padding = 10;
		static constexpr int _y_padding = 10;

		units::ScreenRect _bounds;
		std::string _title;
		std::vector<std::string> _options;
		Continuation<boost::optional<int>> _cont;

		sdl::Texture::ptr _txt_title;
		std::vector<sdl::Texture::ptr> _txt_options;

		int _selection;

		void refresh() override { load_textures(); }

		void load_textures()
		{
			static auto list_option_font_handle = sdl::font_manager().add("resources/fonts/dumbledor1.ttf", 20);

			_txt_title = make_title(_title);
			_bounds.w = _txt_title->width();
			_txt_options.clear();
			for (auto const& option : _options) {
				_txt_options.push_back(std::make_unique<sdl::Texture>(sdl::font_manager()[list_option_font_handle].render(option, sdl::Color::black())));
				_bounds.w = std::max(_bounds.w, _txt_options.back()->width());
			}
			_bounds.w += 2 * _x_padding;
			_bounds.h = _title_height + static_cast<int>(_options.size() * _option_height) + 2 * _y_padding;

			// Confine bounds to window.
			if (_bounds.x + _bounds.w > sdl::window().width()) {
				_bounds.x -= _bounds.w;
			}
			if (_bounds.y + _bounds.h > sdl::window().height()) {
				_bounds.y -= _bounds.h;
			}
		}
	};
}
