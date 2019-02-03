//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <memory>

#include "entities/entity.hpp"
#include "utility/id.hpp"

namespace ql {
	//! An inanimate entity.
	struct object : entity {
		using ptr_less_t = std::function<bool(uptr<object> const&, uptr<object> const&)>;
		using ref_less_t = std::function<bool(object const&, object const&)>;

		id<object> const id;

		virtual ~object() = default;

		//! Whether the object blocks the movement of other entities.
		virtual bool blocks_movement() const = 0;

		//! Advanced this object in time by @p elapsed.
		void update(tick elapsed) final;

	protected:
		object(ql::id<object> id) : entity{}, id{id} {}
	};

	DEFINE_ELEMENT_BASE(object, entity)
}
