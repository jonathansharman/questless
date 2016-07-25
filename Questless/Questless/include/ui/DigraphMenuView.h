/**
* @file    DigraphMenuView.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the DigraphMenuView class.
*/

#ifndef DIGRAPH_MENU_VIEW_H
#define DIGRAPH_MENU_VIEW_H

#include <future>
#include <memory>

#include "sdl-wrappers/basic-sdl-wrappers.h"
#include "sdl-wrappers/Texture.h"
#include "sdl-wrappers/Font.h"
#include "sdl-wrappers/Renderable.h"
#include "DigraphMenuModel.h"
#include "utility/Initializer.h"

namespace questless
{
	class DigraphMenuView : public sdl::Renderable
	{
	public:
		struct PageView
		{
			sdl::Texture title_texture;
			mutable std::vector<sdl::Texture> option_textures;

			PageView(sdl::Texture title_texture, std::vector<sdl::Texture> option_textures)
				: title_texture{std::move(title_texture)}, option_textures{std::move(option_textures)}
			{}
		};

		static constexpr int title_height = 60;
		static constexpr int option_height = 32;
		static constexpr int title_font_size = 48;
		static constexpr int option_font_size = 30;

		/// @param min_width The minimum width of the menu, including margins. If necessary, the menu will stretch to fit its contents.
		/// @param min_height The minimum height of the menu, including margins. If necessary, the menu will stretch to fit its contents.
		DigraphMenuView(int min_width, int min_height)
			: _content_position{0, 0}, _page_index{0}, _current_option_index{0}, _min_width{min_width}, _min_height{min_height}, _content_width{0}, _content_height{0}, _render_is_current{false}
		{}

		/// @return The page view at the specified index.
		const PageView& page(size_t index) const { return _page_views[index]; }

		/// @return A constant reference to the vector of page views.
		const std::vector<PageView>& pages() const { return _page_views; }

		/// @return The coordinate of the upper-left corner of the content region (which excludes margins) of the most recently rendered menu.
		sdl::Point content_position() const { return _content_position; }

		/// @return Whether the menu needs to be rerendered.
		bool render_is_current() const { return _render_is_current; }
		/// @param value Indicates that the menu needs to be rerendered before it is drawn.
		void invalidate_render() { _render_is_current = false; }

		/// Renders or rerenders menu view textures based on the provided menu.
		/// @param menu The menu to be rendered.
		void render(const DigraphMenuModel& menu);

		/// Updates the menu view's page and option indices to match the menu's.
		void update_indices(const DigraphMenuModel& menu);

		/// Gets the position of the menu view.
		sdl::Point position() const { return sdl::Point(_content_position.x - _left_margin.get(), _content_position.y - _top_margin.get()); }

		/// Sets the position of the menu view.
		/// @param origin The origin point of the menu on the screen.
		/// @param horizontal_alignment The horizontal alignment of the menu relative to the origin point.
		/// @param vertical_alignment The vertical alignment of the menu relative to the origin point.
		void position(sdl::Point origin, sdl::HAlign horizontal_alignment = sdl::HAlign::left, sdl::VAlign vertical_alignment = sdl::VAlign::top);

		/// Draws the most recently rendered menu.
		void draw() const;
	private:
		friend class Initializer<DigraphMenuView>;
		static Initializer<DigraphMenuView> _initializer;
		static void initialize();

		static sdl::Handle<sdl::Font> _title_font_handle, _option_font_handle;
		static sdl::Handle<sdl::Texture> _ul_handle, _ur_handle, _dl_handle, _dr_handle, _u_handle, _d_handle, _l_handle, _r_handle, _tile_handle;

		static std::shared_future<int> _top_margin, _bottom_margin, _left_margin, _right_margin, _tile_width, _tile_height;

		static sdl::Color title_color() { return sdl::Color::black(); }
		static sdl::Color unselected_color() { return sdl::Color::black(); }
		static sdl::Color selected_color() { return sdl::Color::red(); }

		std::vector<PageView> _page_views;
		int _page_index;
		int _current_option_index;
		sdl::Point _content_position;
		int _min_width, _min_height;
		int _content_width, _content_height;
		sdl::Texture::ptr _background;
		bool _render_is_current;

		void refresh() override { _render_is_current = false; }
	};
}

#endif
