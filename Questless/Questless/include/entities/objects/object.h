//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <iostream>
#include <memory>

#include "entities/entity.h"
#include "utility/id.h"

namespace ql
{
	//! An inanimate entity.
	class object : public entity
	{
	public:
		///////////
		// Types //
		///////////

		using ptr_less_t = std::function<bool(uptr<object> const&, uptr<object> const&)>;
		using ref_less_t = std::function<bool(object const&, object const&)>;

		/////////////////
		// Public Data //
		/////////////////

		id<object> id;

		////////////////////
		// Public Methods //
		////////////////////

		virtual ~object() = default;

		void serialize(std::ostream& out) const override;

		//! Whether the object blocks the movement of other entities.
		virtual bool blocks_movement() const = 0;

		void update() final;
	protected:
		object(ql::id<object> id) : entity{}, id{id} {}
		object(std::istream& in);
	};

	DEFINE_ELEMENT_BASE(object, entity)
}
