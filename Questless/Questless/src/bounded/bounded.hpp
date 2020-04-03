//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include <cereal/cereal.hpp>

#include <compare>

#define DEFINE_BINARY_OPERATOR(op)                         \
	auto operator op(Derived const& that) const->Derived { \
		return get() op that.get();                    \
	}                                                      \
	template <typename T>                                  \
	auto operator op(T const& that) const->Derived {       \
		return get() op that;                            \
	}                                                      \
	auto operator op##=(Derived const& that) {             \
		set(get() op that.get());                \
		return static_cast<Derived&>(*this);               \
	}                                                      \
	template <typename T>                                  \
	auto operator op##=(T const& that) {                   \
		set(get() op that);                        \
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

		constexpr auto get() const {
			return static_cast<Derived const&>(*this).get();
		}

		constexpr auto set(value_type const& value) -> void {
			static_cast<Derived&>(*this).set(value);
		}
		constexpr auto set(value_type&& value) -> void {
			static_cast<Derived&>(*this).set(std::move(value));
		}

	public:
		//! The contained value.
		constexpr operator value_type() const {
			return get();
		}

		// Comparison Operators

		auto operator<=>(Derived const& that) const {
			return get() <=> that.get();
		}
		auto operator==(Derived const& that) const {
			return get() <=> that.get();
		}

		template <typename OtherValueType>
		auto operator<=>(OtherValueType const& v) const {
			return get() <=> v;
		}
		template <typename OtherValueType>
		auto operator==(OtherValueType const& v) const {
			return get() == v;
		}

		// Arithmetic Operators

		auto operator++() -> Derived& {
			set(get() + 1);
			return *this;
		}
		auto operator--() -> Derived& {
			set(get() - 1);
			return *this;
		}
		auto operator++(int) -> Derived {
			value_type old_value = get();
			set(get() + 1);
			return old_value;
		}
		auto operator--(int) -> Derived {
			value_type old_value = get();
			set(get() - 1);
			return old_value;
		}

		auto operator-() const -> Derived {
			return -get();
		}

		DEFINE_BINARY_OPERATOR(+)
		DEFINE_BINARY_OPERATOR(-)
		DEFINE_BINARY_OPERATOR(*)
		DEFINE_BINARY_OPERATOR(/)
		DEFINE_BINARY_OPERATOR(%)

		// Bit-wise Operators

		auto operator~() const -> Derived {
			return ~(get());
		}

		DEFINE_BINARY_OPERATOR(&);
		DEFINE_BINARY_OPERATOR(|);
		DEFINE_BINARY_OPERATOR(^);

		template <typename T>
		auto operator>>=(int shift) -> Derived& {
			set(get() >> shift);
			return *this;
		}
		auto operator>>(int shift) const -> Derived {
			return get() >> shift;
		}

		template <typename T>
		auto operator<<=(int shift) -> Derived& {
			set(get() << shift);
			return *this;
		}
		auto operator<<(int shift) const -> Derived {
			return get() << shift;
		}

		// Serialization

		template <typename Archive>
		auto save(Archive& archive) const -> void {
			archive(cereal::make_nvp("value", get()));
		}

		template <typename Archive>
		auto load(Archive& archive) -> void {
			value_type value;
			archive(cereal::make_nvp("value", value));
			set(std::move(value));
		}
	};
}

#undef DEFINE_BINARY_OPERATOR
