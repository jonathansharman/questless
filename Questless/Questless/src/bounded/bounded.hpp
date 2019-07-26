//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <cereal/cereal.hpp>

#include <compare>

#define DEFINE_BINARY_OPERATOR(op)                   \
	Derived operator op(Derived const& that) const { \
		return value() op that.value();              \
	}                                                \
	template <typename T>                            \
	Derived operator op(T const& that) const {       \
		return value() op that;                      \
	}                                                \
	Derived& operator op##=(Derived const& that) {   \
		set_value(value() op that.value());          \
		return static_cast<Derived&>(*this);         \
	}                                                \
	template <typename T>                            \
	Derived& operator op##=(T const& that) {         \
		set_value(value() op that);                  \
		return static_cast<Derived&>(*this);         \
	}

namespace ql {
	//! CRTP base class for bounded/property types. Cannot be used directly.
	//! @tparam ValueType The underlying type.
	//! @tparam Derived The derived @p bounded type.
	template <typename ValueType, typename Derived>
	struct bounded {
	private:
		using value_type = ValueType;

		constexpr value_type value() const {
			return static_cast<Derived const&>(*this).value();
		}

		constexpr void set_value(value_type const& value) {
			static_cast<Derived&>(*this).set_value(value);
		}
		constexpr void set_value(value_type&& value) {
			static_cast<Derived&>(*this).set_value(std::move(value));
		}

	public:
		//! The contained value.
		constexpr operator value_type() const {
			return value();
		}

		// Comparison Operators

		friend auto operator<=>(Derived const& d1, Derived const& d2) {
			return d1.value() <=> d2.value();
		}
		friend auto operator==(Derived const& d1, Derived const& d2) {
			return d1.value() <=> d2.value();
		}

		template <typename OtherValueType>
		friend auto operator<=>(Derived const& d, OtherValueType const& v) {
			return d.value() <=> v;
		}
		template <typename OtherValueType>
		friend auto operator==(Derived const& d, OtherValueType const& v) {
			return d.value() == v;
		}

		template <typename OtherValueType>
		friend auto operator<=>(OtherValueType const& v, Derived const& d) {
			return v <=> d.value();
		}
		template <typename OtherValueType>
		friend auto operator==(OtherValueType const& v, Derived const& d) {
			return v == d.value();
		}

		// Arithmetic Operators

		Derived& operator++() {
			set_value(value() + 1);
			return *this;
		}
		Derived& operator--() {
			set_value(value() - 1);
			return *this;
		}
		Derived operator++(int) {
			value_type old_value = value();
			set_value(value() + 1);
			return old_value;
		}
		Derived operator--(int) {
			value_type old_value = value();
			set_value(value() - 1);
			return old_value;
		}

		Derived operator-() const {
			return -value();
		}

		DEFINE_BINARY_OPERATOR(+)
		DEFINE_BINARY_OPERATOR(-)
		DEFINE_BINARY_OPERATOR(*)
		DEFINE_BINARY_OPERATOR(/)
		DEFINE_BINARY_OPERATOR(%)

		// Bit-wise Operators

		Derived operator~() const {
			~(value());
		}

		DEFINE_BINARY_OPERATOR(&);
		DEFINE_BINARY_OPERATOR(|);
		DEFINE_BINARY_OPERATOR(^);

		template <typename T>
		Derived& operator>>=(int shift) {
			set_value(value() >> shift);
			return *this;
		}
		Derived operator>>(int shift) const {
			return value() >> shift;
		}

		template <typename T>
		Derived& operator<<=(int shift) {
			set_value(value() << shift);
			return *this;
		}
		Derived operator<<(int shift) const {
			return value() << shift;
		}

		// Serialization

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
	};
}

#undef DEFINE_BINARY_OPERATOR
