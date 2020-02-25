//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include <cereal/cereal.hpp>

#include <compare>

#define DEFINE_BINARY_OPERATOR(op)                         \
	auto operator op(Derived const& that) const->Derived { \
		return value() op that.value();                    \
	}                                                      \
	template <typename T>                                  \
	auto operator op(T const& that) const->Derived {       \
		return value() op that;                            \
	}                                                      \
	auto operator op##=(Derived const& that) {             \
		set_value(value() op that.value());                \
		return static_cast<Derived&>(*this);               \
	}                                                      \
	template <typename T>                                  \
	auto operator op##=(T const& that) {                   \
		set_value(value() op that);                        \
		return static_cast<Derived&>(*this);               \
	}

namespace ql {
	//! CRTP base class for bounded/property types. Cannot be used directly.
	//! @tparam ValueType The underlying type.
	//! @tparam Derived The derived @p bounded type.
	template <typename ValueType, typename Derived>
	struct bounded {
	private:
		using value_type = ValueType;

		constexpr auto value() const {
			return static_cast<Derived const&>(*this).value();
		}

		constexpr auto set_value(value_type const& value) -> void {
			static_cast<Derived&>(*this).set_value(value);
		}
		constexpr auto set_value(value_type&& value) -> void {
			static_cast<Derived&>(*this).set_value(std::move(value));
		}

	public:
		//! The contained value.
		constexpr operator value_type() const {
			return value();
		}

		// Comparison Operators

		auto operator<=>(Derived const& that) const {
			return value() <=> that.value();
		}
		auto operator==(Derived const& that) const {
			return value() <=> that.value();
		}

		template <typename OtherValueType>
		auto operator<=>(OtherValueType const& v) const {
			return value() <=> v;
		}
		template <typename OtherValueType>
		auto operator==(OtherValueType const& v) const {
			return value() == v;
		}

		// Arithmetic Operators

		auto operator++() -> Derived& {
			set_value(value() + 1);
			return *this;
		}
		auto operator--() -> Derived& {
			set_value(value() - 1);
			return *this;
		}
		auto operator++(int) -> Derived {
			value_type old_value = value();
			set_value(value() + 1);
			return old_value;
		}
		auto operator--(int) -> Derived {
			value_type old_value = value();
			set_value(value() - 1);
			return old_value;
		}

		auto operator-() const -> Derived {
			return -value();
		}

		DEFINE_BINARY_OPERATOR(+)
		DEFINE_BINARY_OPERATOR(-)
		DEFINE_BINARY_OPERATOR(*)
		DEFINE_BINARY_OPERATOR(/)
		DEFINE_BINARY_OPERATOR(%)

		// Bit-wise Operators

		auto operator~() const -> Derived {
			return ~(value());
		}

		DEFINE_BINARY_OPERATOR(&);
		DEFINE_BINARY_OPERATOR(|);
		DEFINE_BINARY_OPERATOR(^);

		template <typename T>
		auto operator>>=(int shift) -> Derived& {
			set_value(value() >> shift);
			return *this;
		}
		auto operator>>(int shift) const -> Derived {
			return value() >> shift;
		}

		template <typename T>
		auto operator<<=(int shift) -> Derived& {
			set_value(value() << shift);
			return *this;
		}
		auto operator<<(int shift) const -> Derived {
			return value() << shift;
		}

		// Serialization

		template <typename Archive>
		auto save(Archive& archive) const -> void {
			archive(cereal::make_nvp("value", value()));
		}

		template <typename Archive>
		auto load(Archive& archive) -> void {
			value_type value;
			archive(cereal::make_nvp("value", value));
			set_value(std::move(value));
		}
	};
}

#undef DEFINE_BINARY_OPERATOR
