/**
* @file    Body.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Body class, which represents a being's body. Holds a BodyPart tree.
*/

#ifndef BODY_H
#define BODY_H

#include <memory>

#include "entities/beings/BodyPart.h"
#include "sdl-wrappers/basic-sdl-wrappers.h"
#include "utility/Stream.h"

namespace questless
{
	class Body
	{
	public:
		using ptr = std::unique_ptr<Body>;

		/// @param root The root of the body parts tree.
		Body(std::unique_ptr<BodyPart> root);

		Body(Body&&) = default;
		
		~Body();

		/// @return The root body part.
		BodyPart& root() { return *_root; }

		/// @return A stream of all the body's parts.
		Stream<const BodyPart*> parts() const;
		/// @return A stream of all the body's parts.
		Stream<BodyPart*> parts();

		/// @return The number of body parts this body has.
		size_t parts_count() const { return _parts_count; }

		/// @return The bounding box around the body's parts.
		sdl::Rect bounds() const { return _bounds; }

		/// @return The offset from the upper left corner of the bounds to the body's center.
		sdl::Vector offset_to_center() const { return _offset_to_center; }
	private:
		std::unique_ptr<BodyPart> _root;

		size_t _parts_count;
		sdl::Rect _bounds;
		sdl::Vector _offset_to_center;
	};
}

#endif
