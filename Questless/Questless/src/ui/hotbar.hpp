//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

#include "reg.hpp"

#include <optional>

namespace ql {
	//! Holds frequently used items.
	struct hotbar : widget {
		hotbar(widget& parent);

		view::vector get_size() const final;

		void update(sec elapsed_time, std::vector<sf::Event>& events) final;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

		auto const& o_item_ids() const {
			return _o_item_ids;
		}

		//! Gets the most recent selections, oldest to newest, and then clears the selection buffer.
		std::vector<int> poll_selections();

		//! The most recent selection made or 0 if no selection has been made yet.
		auto most_recent_selection() const;

	private:
		static constexpr size_t _item_count = 10;

		std::array<std::optional<ent>, 10> _o_item_ids;
		std::vector<int> _selections;
		int _most_recent_selection = 0;

		sf::Texture _slot_texture;

		void add(int selection);
	};
}
