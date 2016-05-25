/**
* @file    Object.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Object class, which includes all inanimate entities.
*/

#ifndef OBJECT_H
#define OBJECT_H

#include <memory>

#include "entities/Entity.h"

namespace questless
{
	namespace ObjectK
	{
	}

	class Object : public Entity
	{
	public:
		///////////
		// Types //
		///////////

		using ptr = std::unique_ptr<Object>;
		using ptr_less_t = std::function<bool(const std::unique_ptr<Object>&, const std::unique_ptr<Object>&)>;
		using ref_less_t = std::function<bool(const Object&, const Object&)>;

		/////////////////
		// Public Data //
		/////////////////

		////////////////////
		// Public Methods //
		////////////////////

		virtual ~Object() = default;

		Object* as_object() { return dynamic_cast<Object*>(this); }

		/// @param out A stream object into which the serialized object is inserted.
		void serialize(std::ostream& out) const override;

		/// Advances the object one time unit.
		void update() override;
	protected:
		Object();
		Object(std::istream& in);
	private:
	};
}

#endif