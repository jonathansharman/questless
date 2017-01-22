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
#include "entities/objects/ObjectId.h"

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

		////////////////////
		// Public Methods //
		////////////////////

		virtual ~Object() = default;

		/// @param out A stream object into which the serialized object is inserted.
		void serialize(std::ostream& out) const override;

		ObjectId id() const { return _id; }

		/// Advances the object one time unit.
		void update() override;
	protected:
		Object(Game& game, ObjectId id) : Entity(game), _id{id} {}
		Object(Game& game, std::istream& in);
	private:
		ObjectId _id;
	};
}
