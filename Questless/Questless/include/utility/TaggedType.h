/**
* @file    TaggedType.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION Supports creation of strongly typed wrappers around MoveConstructible types.
*/

#ifndef TAGGED_TYPE_H
#define TAGGED_TYPE_H

namespace questless
{
	/// Adds a type tag to an existing type.
	template <typename RawType, typename TagType>
	class TaggedType
	{
	public:
		using raw_t = RawType;
		using tag_t = TagType;

		constexpr explicit TaggedType(raw_t value) : _value{std::move(value)} {}

		constexpr const raw_t& value() const & { return _value; }
		raw_t&& value() && { return std::move(_value); }
		raw_t& value() & { return _value; }

		constexpr operator const raw_t&() const & { return _value; }
		operator raw_t && () && { return std::move(_value); }
	private:
		raw_t _value;
	};
}

#endif
