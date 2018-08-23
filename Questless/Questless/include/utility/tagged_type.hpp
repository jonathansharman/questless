//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

namespace ql {
	//! @brief Supports creation of strongly typed wrappers around MoveConstructible types.
	//!
	//! An object of type tagged_type<T> can be used like an object of type T, but passing a T as a
	//! tagged_type<T> requires explicit conversion. This is useful for making parameters more type-safe.
	//!
	//! Example:
	//! @code
	//! struct meters : tagged_type<double> { using tagged_type::tagged_type; };
	//! @endcode
	template <typename RawType>
	struct tagged_type {
		using raw_type = RawType;

		raw_type value;

		constexpr explicit tagged_type(raw_type value) : value{std::move(value)} {}

		constexpr operator raw_type const&() const& noexcept { return value; }
		operator raw_type&() & noexcept { return value; }

		constexpr const raw_type* operator ->() const { return &value; }
		raw_type* operator ->() { return &value; }
	};
}
