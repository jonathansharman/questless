//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

#include "reg.hpp"
#include "utility/reference.hpp"

#include <optional>

namespace ql {
	namespace rsrc {
		struct tile;
	}
	struct animation;

	//! Allows interaction with a tile in the world.
	struct tile_widget : widget {
		//! The tile this widget interfaces with.
		std::optional<id> o_tile_id;

		tile_widget(rsrc::tile const& resources);

		auto get_size() const -> view::vector final;

		auto update(sec elapsed_time) -> void final;

		auto set_position(view::point position) -> void final;

		auto get_position() const -> view::point final;

		//! Updates this tile's appearance. Call only when the tile's state may have changed.
		auto render() -> void;

	private:
		rsrc::tile const& _rsrc;

		view::point _position;
		uptr<animation> _ani;

		auto draw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;
	};
}
