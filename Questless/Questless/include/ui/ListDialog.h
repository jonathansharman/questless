/**
* @file    ListDialog.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the ListDialog class, which allows the player to choose from a list of options.
*/

#ifndef LIST_DIALOG_H
#define LIST_DIALOG_H

#include <string>
#include <vector>

#include "Dialog.h"

namespace questless
{
	template <typename ItemType>
	class ListDialog : public Dialog
	{
	public:
		ListDialog
			( sdl::Point origin
			, std::string title
			, std::vector<ItemType> options
			, std::function<std::string(const ItemType&)> item_to_name
			, std::function<void(boost::optional<ItemType>)> cont
			)
			: _bounds{origin.x, origin.y, 0, 0}
			, _title{std::move(title)}
			, _options{std::move(options)}
			, _item_to_name{std::move(item_to_name)}
			, _cont{std::move(cont)}
			, _selection{0}
		{
			load_textures();
		}

		/// Updates the dialog state based on input.
		/// @param input User input used to update the dialog.
		void update(const sdl::Input& input) override
		{
			if (input.presses(SDLK_BACKSPACE) || input.presses(SDLK_ESCAPE)) {
				close();
				return _cont(boost::none);
			}

			const size_t option_count = _options.size();

			if (!_options.empty()) {
				_selection -= input.presses(SDLK_UP);
				_selection += input.presses(SDLK_DOWN);
				_selection = _selection % static_cast<int>(option_count);
				_selection = _selection < 0 ? _selection + option_count : _selection;

				size_t end = option_count <= 10 ? option_count : 10;
				for (size_t i = 0; i < end; ++i) {
					if (input.presses(sdl::Input::index_to_num_key(i))) {
						if (_selection == static_cast<int>(i)) {
							close();
							return _cont(std::move(_options[_selection]));
						} else {
							_selection = i;
							break;
						}
					}
				}

				if (input.pressed(sdl::MouseButton::left) || input.presses(SDLK_RETURN) || input.presses(SDLK_SPACE)) {
					close();
					return _cont(std::move(_options[_selection]));
				}
			}
		}

		/// Draws the dialog to the screen.
		/// @param window The window.
		void draw(const sdl::Window& window) override
		{
			/// @todo Confine to game window in the refresh function (need access to the window, will probably need to store the window dimensions :( ).

			// Confine to game window.
			if (_bounds.x + _bounds.w > window.width()) {
				_bounds.x -= _bounds.w;
			}
			if (_bounds.y + _bounds.h > window.height()) {
				_bounds.y -= _bounds.h;
			}

			// Draw background.
			sdl::renderer().draw_rect(_bounds, sdl::Color::black(), sdl::Color{255, 200, 150});

			// Draw highlight.
			sdl::renderer().draw_rect(sdl::Rect{_bounds.x + _x_padding, _bounds.y + _y_padding + _title_height + _selection * _option_height, _bounds.w - 2 * _x_padding, _option_height}, sdl::Color::white(), true);

			// Draw title.
			_txt_title->draw(sdl::Point{_bounds.x + _x_padding, _bounds.y + _y_padding});

			// Draw options.
			for (size_t i = 0; i < _options.size(); ++i) {
				int y = _bounds.y + _y_padding + _title_height + static_cast<int>(i) * _option_height;
				_txt_options[i]->draw(sdl::Point{_bounds.x + _x_padding, y});
			}
		}
	private:
		static constexpr int _title_height = 40;
		static constexpr int _option_height = 20;
		static constexpr int _x_padding = 10;
		static constexpr int _y_padding = 10;

		sdl::Rect _bounds;
		std::string _title;
		std::vector<ItemType> _options;
		std::function<std::string(const ItemType&)> _item_to_name;
		std::function<void(boost::optional<ItemType>)> _cont;

		sdl::Texture::ptr _txt_title;
		std::vector<sdl::Texture::ptr> _txt_options;

		int _selection;

		void refresh() override { load_textures(); }

		void load_textures()
		{
			_txt_title = sdl::Texture::make(sdl::font_manager()[title_font_handle()].render(_title, sdl::renderer(), sdl::Color::black()));
			_bounds.w = _txt_title->width();
			_txt_options.clear();
			for (const auto& option : _options) {
				_txt_options.push_back(sdl::Texture::make(sdl::font_manager()[list_option_font_handle()].render(_item_to_name(option), sdl::renderer(), sdl::Color::black())));
				_bounds.w = std::max(_bounds.w, _txt_options.back()->width());
			}
			_bounds.w += 2 * _x_padding;
			_bounds.h = _title_height + static_cast<int>(_options.size() * _option_height) + 2 * _y_padding;
		}
	};
}

#endif
