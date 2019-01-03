//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "dialog.hpp"

#include "sdl/resources.hpp"
#include "sdl/spaces/window.hpp"

#include <string>
#include <vector>

namespace ql {
	//! Retrieves the player's choice from a list of options.
	class list_dialog : public dialog {
	public:
		list_dialog
			( sdl::spaces::window::point origin
			, std::string title
			, std::vector<std::string> options
			, std::function<void(std::optional<int>)> cont
			)
			: _bounds{origin, sdl::spaces::window::vector::zero()}
			, _title{std::move(title)}
			, _options{std::move(options)}
			, _cont{std::move(cont)}
			, _selection{0}
		{
			load_textures();
		}

		state update() final {
			if (sdl::the_input().presses(SDLK_BACKSPACE) || sdl::the_input().presses(SDLK_ESCAPE)) {
				return _cont(std::nullopt);
			}

			int const option_count = static_cast<int>(_options.size());

			if (!_options.empty()) {
				_selection -= sdl::the_input().presses(SDLK_UP);
				_selection += sdl::the_input().presses(SDLK_DOWN);
				_selection += sdl::the_input().scroll();
				_selection = _selection % option_count;
				_selection = _selection < 0 ? _selection + option_count : _selection;

				int const end = option_count <= 10 ? option_count : 10;
				for (int i = 0; i < end; ++i) {
					if (sdl::the_input().presses(sdl::input::index_to_num_key(i))) {
						if (_selection == i) {
							return _cont(_selection);
						} else {
							_selection = i;
							break;
						}
					}
				}

				if (sdl::the_input().pressed(sdl::mouse_button::left) || sdl::the_input().presses(SDLK_RETURN) || sdl::the_input().presses(SDLK_SPACE)) {
					return _cont(_selection);
				}
			}

			return state::open;
		}

		void draw() const final {
			using namespace sdl::spaces;
			using namespace sdl::spaces::colors;
			using namespace sdl::spaces::colors::literals;
			using namespace sdl::spaces::window::literals;

			// Draw background.
			sdl::the_renderer().draw_box(_bounds, 1_px, black(), color{1.0_c, 0.75_c, 0.6_c, 1.0_c});

			{ // Draw highlight.
				sdl::spaces::window::box const highlight_bounds
					{ _bounds.position + _padding + sdl::spaces::window::vector
						{ sdl::spaces::window::px{0}
						, sdl::spaces::window::px{_title_height + _selection * _option_height}
						}
					, window::vector{width(_bounds) - 2 * _padding[0], _option_height}
					};
				sdl::the_renderer().draw_box(highlight_bounds, sdl::spaces::colors::white());
			}

			// Draw title.
			_txt_title->draw(_bounds.position + _padding);

			// Draw options.
			for (std::size_t i = 0; i < _options.size(); ++i) {
				auto const position = _bounds.position + _padding + sdl::spaces::window::vector
					{ sdl::spaces::window::px{0}
					, sdl::spaces::window::px{_title_height + static_cast<int>(i) * _option_height}
					};
				_txt_options[i].draw(position);
			}
		}
	private:
		static constexpr auto _title_height = sdl::spaces::window::px{40};
		static constexpr auto _option_height = sdl::spaces::window::px{20};
		static constexpr auto _padding = sdl::spaces::window::vector{sdl::spaces::window::px{10}, sdl::spaces::window::px{10}};

		sdl::spaces::window::box _bounds;
		std::string _title;
		std::vector<std::string> _options;
		continuation<std::optional<int>> _cont;

		uptr<sdl::texture> _txt_title;
		std::vector<sdl::texture> _txt_options;

		int _selection;

		void load_textures() {
			using namespace sdl::spaces::colors;

			static auto list_option_font_handle = sdl::the_font_manager().add("resources/fonts/dumbledor1.ttf", 20);

			_txt_title = make_title(_title.c_str(), sdl::spaces::colors::black());
			width(_bounds) = _txt_title->width() * sdl::spaces::window::px{1} / sdl::spaces::window::px{1};
			_txt_options.clear();
			for (auto const& option : _options) {
				_txt_options.push_back(sdl::the_font_manager()[list_option_font_handle].render(option.c_str(), black()));
				width(_bounds) = std::max(width(_bounds), _txt_options.back().width() * sdl::spaces::window::px{1} / sdl::spaces::window::px{1});
			}
			_bounds.size += sdl::spaces::window::vector
				{ 2 * _padding[0]
				, _title_height + static_cast<int>(_options.size()) * _option_height + 2 * _padding[1]
				};

			// Confine bounds to window.
			if (right(_bounds) > sdl::the_window().width()) {
				left(_bounds) -= width(_bounds);
			}
			if (bottom(_bounds) > sdl::the_window().height()) {
				top(_bounds) -= height(_bounds);
			}
		}
	};
}
