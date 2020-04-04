//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "widget.hpp"

#include "entities/beings/world_view.hpp"
#include "reg.hpp"
#include "rsrc/entity_fwd.hpp"
#include "rsrc/particle_fwd.hpp"
#include "utility/reference.hpp"

#include <optional>

namespace ql {
	struct animation;

	//! Allows interaction with an entity.
	struct entity_widget : widget {
		//! @param entity_view A view of the entity this widget interfaces with.
		entity_widget( //
			reg& reg,
			rsrc::entity const& entity_resources,
			rsrc::particle const& particle_resources,
			world_view::entity_view entity_view);

		auto get_size() const -> view::vector final;

		auto update(sec elapsed_time) -> void final;

		auto set_position(view::point position) -> void final;

		auto get_position() const -> view::point final;

	private:
		reg_ptr _reg;

		rsrc::entity_ptr _entity_resources;
		rsrc::particle_ptr _particle_resources;

		world_view::entity_view _ev;
		view::point _position;
		uptr<animation> _ani;

		auto draw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;
	};
}
