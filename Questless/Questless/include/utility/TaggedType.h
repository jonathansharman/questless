/**
* @file    TaggedType.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION
*
* Supports creation of strongly typed wrappers around MoveConstructible types.
*
* Example tagged type definition:
*   struct Meters : TaggedType<double> { using TaggedType::TaggedType; };
*/

#ifndef TAGGED_TYPE_H
#define TAGGED_TYPE_H

namespace questless
{
	template <typename RawType>
	struct TaggedType
	{
		using raw_t = RawType;

		raw_t value;

		constexpr explicit TaggedType(raw_t value) : value{std::move(value)} {}

		constexpr operator const raw_t&() const & noexcept { return value; }
		operator raw_t&() & noexcept { return value; }

		constexpr const raw_t* operator ->() const { return &value; }
		raw_t* operator ->() { return &value; }
	};
}

#endif
