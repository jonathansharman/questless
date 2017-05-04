//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

namespace questless
{
	//! @brief Supports creation of strongly typed wrappers around MoveConstructible types.
	//!
	//! An object of type TaggedType<T> can be used like an object of type T, but passing a T as a
	//! TaggedType<T> requires explicit conversion. This is useful for making parameters more type-safe.
	//!
	//! Example:
	//! @code
	//! struct Meters : TaggedType<double> { using TaggedType::TaggedType; };
	//! @endcode
	template <typename RawType>
	struct TaggedType
	{
		using raw_t = RawType;

		raw_t value;

		constexpr explicit TaggedType(raw_t value) : value{std::move(value)} {}

		constexpr operator raw_t const&() const& noexcept { return value; }
		operator raw_t&() & noexcept { return value; }

		constexpr const raw_t* operator ->() const { return &value; }
		raw_t* operator ->() { return &value; }
	};
}
