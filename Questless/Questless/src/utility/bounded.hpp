//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <cereal/cereal.hpp>

#define DEFINE_COMPARISON_OPERATOR(op) \
	constexpr bool operator op(Derived const& that) const { return value() == that.value(); } \
	\
	template <typename T> \
	constexpr bool operator op(T const& that) const { return value() op that; }

#define DEFINE_BINARY_OPERATOR(op) \
	Derived operator op(Derived const& that) const { return value() op that.value(); } \
	\
	template <typename T> \
	Derived operator op(T const& that) const { return value() op that; } \
	\
	Derived& operator op##=(Derived const& that) { \
		set_value(value() op that.value()); \
		return static_cast<Derived&>(*this); \
	} \
	\
	template <typename T> \
	Derived& operator op##=(T const& that) { \
		set_value(value() op that); \
		return static_cast<Derived&>(*this); \
	}

namespace ql {
	//! CRTP base class for bounded/property types. Cannot be used directly.
	//! @tparam ValueType The underlying type.
	template <typename ValueType, typename Derived>
	class bounded {
	private:
		using value_type = ValueType;

		constexpr value_type value() const { return static_cast<Derived const&>(*this).value(); }

		constexpr void set_value(value_type const& value) { static_cast<Derived&>(*this).set_value(value); }
		constexpr void set_value(value_type&& value) { static_cast<Derived&>(*this).set_value(std::move(value)); }
	protected:
		constexpr bounded() = default;
	public:
		template <typename Archive>
		void save(Archive& archive) const {
			archive(cereal::make_nvp("value", value()));
		}

		template <typename Archive>
		void load(Archive& archive) {
			value_type value;
			archive(cereal::make_nvp("value", value));
			set_value(std::move(value));
		}

		//! The contained value.
		constexpr operator value_type() const { return value(); }

		// Comparison Operators
		
		DEFINE_COMPARISON_OPERATOR(==)
		DEFINE_COMPARISON_OPERATOR(!=)
		DEFINE_COMPARISON_OPERATOR(<)
		DEFINE_COMPARISON_OPERATOR(<=)
		DEFINE_COMPARISON_OPERATOR(>)
		DEFINE_COMPARISON_OPERATOR(>=)

		// Arithmetic Operators

		Derived& operator ++() {
			set_value(value() + 1);
			return *this;
		}
		Derived& operator --() {
			set_value(value() - 1);
			return *this;
		}
		Derived operator ++(int) {
			value_type old_value = value();
			set_value(value() + 1);
			return old_value;
		}
		Derived operator --(int) {
			value_type old_value = value();
			set_value(value() - 1);
			return old_value;
		}

		Derived operator -() const { return -value(); }

		DEFINE_BINARY_OPERATOR(+)
		DEFINE_BINARY_OPERATOR(-)
		DEFINE_BINARY_OPERATOR(*)
		DEFINE_BINARY_OPERATOR(/)
		DEFINE_BINARY_OPERATOR(%)

		// Bit-wise Operators

		Derived operator ~() const { ~(value()); }

		DEFINE_BINARY_OPERATOR(&);
		DEFINE_BINARY_OPERATOR(|);
		DEFINE_BINARY_OPERATOR(^);

		template <typename T>
		Derived& operator >>=(int shift) {
			set_value(value() >> shift);
			return *this;
		}
		Derived operator >>(int shift) const { return value() >> shift; }

		template <typename T>
		Derived& operator <<=(int shift) {
			set_value(value() << shift);
			return *this;
		}
		Derived operator <<(int shift) const { return value() << shift; }
	};
}

#undef DEFINE_COMPARISON_OPERATOR
#undef DEFINE_BINARY_OPERATOR
