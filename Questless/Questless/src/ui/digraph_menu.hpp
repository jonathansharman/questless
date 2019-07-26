//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "input_manager.hpp"
#include "view_space.hpp"
#include "widget.hpp"

#include "rsrc/menu.hpp"
#include "utility/reference.hpp"

#include <optional>

namespace ql {
	namespace rsrc {
		struct fonts;
	}

	//! A menu consisting of linked pages of options.
	struct digraph_menu : widget {
		//! @param min_size The minimum size of the menu including margins. The menu will stretch to fit its contents.
		digraph_menu(widget& parent, rsrc::fonts const& fonts, view::vector min_size);

		//! Adds a blank page to the menu.
		//! @param title The page title.
		//! @note Throws a std::invalid_argument if a page with the same title already exists.
		void add_page(std::string title);

		//! Adds a terminal option (one which doesn't lead to another page) to the specified page of the menu.
		//! @param page_title The title of the page to which the option is to be added.
		//! @param option_name The name of the option to be added.
		//! @note Throws a std::invalid_argument if the page does not exist.
		void add_option(std::string_view page_title, std::string option_name);

		//! Adds a nonterminal option (one which leads to another page) to the specified page of the menu.
		//! @param location_page_title The title of the page to which the option is to be added.
		//! @param option_name The name of the option to be added.
		//! @param target_page_title The title of the page to which the option links.
		void add_option(std::string_view location_page_title, std::string option_name, std::string_view target_page_title);

		//! Selects the specified option on the specified page.
		//! @param page_title The title of the page whose option index is to be set.
		//! @param option_index The index of the option to be selected.
		void set_option(std::string_view page_title, int option_index);

		view::vector get_local_offset() const final;

		view::vector get_size() const final;

		//! Updates the menu.
		void update(sec elapsed_time, std::vector<sf::Event>& events);

		//! Gets all the terminal menu selections since the last call to poll_selections().
		//! @return A vector of pairs of strings, each representing a page name and option name. The pairs are ordered
		//! from least to most recently selected.
		std::vector<std::pair<std::string, std::string>> poll_selections();

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

	private:
		struct page {
			struct option {
				std::string name;
				std::optional<int> target;

				//! @param name The name of the option.
				//! @param target The index of page to which this option links, or nullopt if the option is terminal.
				option(std::string name, std::optional<int> target) : name{std::move(name)}, target{target} {}
			};

			std::string title;
			std::vector<option> options;
			int option_index;

			page(std::string title) : title{std::move(title)}, option_index{0} {}
		};

		struct page_view {
			sf::Texture title_texture;
			mutable std::vector<sf::Texture> option_textures;

			page_view(sf::Texture title_texture, std::vector<sf::Texture> option_textures)
				: title_texture{std::move(title_texture)}, option_textures{std::move(option_textures)} {}
		};

		rsrc::menu resources;
		rsrc::fonts const& fonts;

		view::vector _top_left_margin;
		view::vector _bottom_right_margin;
		view::vector _tile_size;

		std::vector<page> _pages;
		int _page_index = 0;
		std::vector<std::pair<std::string, std::string>> _selections;

		std::vector<page_view> _page_views;
		view::point _content_position{0.0_px, 0.0_px};
		view::vector _min_size;
		view::vector _content_size;
		uptr<sf::Texture> _background;
		bool _render_is_current = false;

		//! Finds the first page with the given page title, if it exists.
		//! @param page_title The title of the page to be found.
		//! @return The index of the page or nullopt if no matching page was found.
		std::optional<int> find(std::string_view page_title);

		//! Renders or rerenders menu textures.
		void render();
	};
}
