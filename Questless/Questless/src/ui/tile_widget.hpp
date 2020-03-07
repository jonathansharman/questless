//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

#include "entities/beings/world_view.hpp"
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
		//! @param entity_view A view of the tile this widget interfaces with.
		tile_widget(reg& reg, rsrc::tile const& resources, world_view::tile_view const& tile_view);

		auto get_size() const -> view::vector final;

		auto update(sec elapsed_time) -> void final;

		auto set_position(view::point position) -> void final;

		auto get_position() const -> view::point final;

	private:
		reg_ptr _reg;

		rsrc::tile const& _rsrc;

		world_view::tile_view _tv;
		view::point _position;
		uptr<animation> _ani;

		auto draw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;
	};
}
