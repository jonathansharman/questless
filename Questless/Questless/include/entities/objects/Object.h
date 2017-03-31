/**
* @file    Object.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION An inanimate entity.
*/

#pragma once

#include <iostream>
#include <memory>

#include "entities/Entity.h"
#include "utility/Id.h"

namespace questless
{
	class Object : public Entity
	{
	public:
		///////////
		// Types //
		///////////

		using ptr = std::unique_ptr<Object>;
		using ref = std::reference_wrapper<Object>;
		using ptr_less_t = std::function<bool(std::unique_ptr<Object> const&, std::unique_ptr<Object> const&)>;
		using ref_less_t = std::function<bool(Object const&, Object const&)>;

		/////////////////
		// Public Data //
		/////////////////

		Id<Object> id;

		////////////////////
		// Public Methods //
		////////////////////

		virtual ~Object() = default;

		/// @param out A stream object into which the serialized object is inserted.
		void serialize(std::ostream& out) const override;

		/// @return Whether the object blocks the movement of other entities.
		////
		virtual bool blocks_movement() const = 0;

		/// Advances the object one time unit.
		void update() override;
	protected:
		Object(Id<Object> id) : Entity{}, id{id} {}
		Object(std::istream& in);
	};
}
