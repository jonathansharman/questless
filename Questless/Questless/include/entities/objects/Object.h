//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <iostream>
#include <memory>

#include "entities/Entity.h"
#include "utility/Id.h"

namespace questless
{
	//! An inanimate entity.
	class Object : public Entity
	{
	public:
		///////////
		// Types //
		///////////

		using uptr = std::unique_ptr<Object>;
		using ref = std::reference_wrapper<Object>;
		using cref = std::reference_wrapper<Object const>;
		using ptr_less_t = std::function<bool(uptr const&, uptr const&)>;
		using ref_less_t = std::function<bool(Object const&, Object const&)>;

		/////////////////
		// Public Data //
		/////////////////

		Id<Object> id;

		////////////////////
		// Public Methods //
		////////////////////

		virtual ~Object() = default;

		void serialize(std::ostream& out) const override;

		//! Whether the object blocks the movement of other entities.
		virtual bool blocks_movement() const = 0;

		void update() override;
	protected:
		Object(Id<Object> id) : Entity{}, id{id} {}
		Object(std::istream& in);
	};
}
