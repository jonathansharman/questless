//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.
//! @brief Utility functions for I/O tasks.

#pragma once

#include "id.hpp"
#include "lazy_bounded.hpp"
#include "nonnegative.hpp"
#include "static_bounded.hpp"

#include "meta/quantity.hpp"
#include "units/space.hpp"

#include <nlohmann/json.hpp>
#include <cereal/cereal.hpp>

#include <string>

namespace ql {
	//! The contents, as a string, of the file with filename @p filename.
	std::string contents_of_file(char const* filename);

	template <typename T>
	void from_json(nlohmann::json j, id<T>& arg) {
		typename id<T>::key_t key;
		nlohmann::adl_serializer<id<T>::key_t>::from_json(j, key);
		arg = id<T>{key};
	}
	template <typename T>
	void to_json(nlohmann::json j, id<T> const& arg) {
		nlohmann::adl_serializer<T>::to_json(j, arg.key());
	}

	template <typename T>
	void from_json(nlohmann::json j, nonnegative<T>& arg) {
		T value;
		nlohmann::adl_serializer<T>::from_json(j, value);
		arg.set_value(std::move(value));
	}
	template <typename T>
	void to_json(nlohmann::json j, nonnegative<T> const& arg) {
		nlohmann::adl_serializer<T>::to_json(j, arg.value());
	}

	template <typename T, T LowerBound, T UpperBound>
	void from_json(nlohmann::json j, static_bounded<T, LowerBound, UpperBound>& arg) {
		T value;
		nlohmann::adl_serializer<T>::from_json(j, value);
		arg.set_value(std::move(value));
	}
	template <typename T, T LowerBound, T UpperBound>
	void to_json(nlohmann::json j, static_bounded<T, LowerBound, UpperBound> const& arg) {
		nlohmann::adl_serializer<T>::to_json(j, arg.value());
	}

	template <typename T>
	void from_json(nlohmann::json j, lazy_bounded<T>& arg) {
		T value;
		nlohmann::adl_serializer<T>::from_json(j, value);
		arg.set_value(std::move(value));
	}
	template <typename T>
	void to_json(nlohmann::json j, lazy_bounded<T> const& arg) {
		to_json(j, arg.value());
	}
}

namespace meta {
	template <typename Rep, typename Unit>
	void from_json(nlohmann::json j, quantity<Rep, Unit>& arg) {
		nlohmann::adl_serializer<Rep>::from_json(j, arg.value);
	}
	template <typename Rep, typename Unit>
	void to_json(nlohmann::json j, quantity<Rep, Unit> const& arg) {
		nlohmann::adl_serializer<Rep>::to_json(j, arg.value);
	}
}

namespace units {
	// angle

	template <typename Tag, typename Scalar, int DimensionCount, typename Buffer, typename UnitsPerCircle>
	void from_json(nlohmann::json j, typename space<Tag, Scalar, DimensionCount, Buffer>::angle<UnitsPerCircle>& arg) {
		if constexpr (DimensionCount == 2) {
			nlohmann::adl_serializer<Scalar>::from_json(j, arg.count());
		} else {
			for (std::size_t i = 0; i < DimensionCount - 1; ++i) {
				nlohmann::adl_serializer<Scalar>::from_json(j[i], arg[i]);
			}
		}
	}
	template <typename Tag, typename Scalar, int DimensionCount, typename Buffer, typename UnitsPerCircle>
	void to_json(nlohmann::json j, typename space<Tag, Scalar, DimensionCount, Buffer>::angle<UnitsPerCircle> const& arg) {
		if constexpr (DimensionCount == 2) {
			nlohmann::adl_serializer<Scalar>::to_json(j, arg[0]);
		} else {
			for (std::size_t i = 0; i < DimensionCount - 1; ++i) {
				nlohmann::adl_serializer<Scalar>::to_json(j[i], arg[i]);
			}
		}
	}

	// point

	template <typename Tag, typename Scalar, int DimensionCount, typename Buffer>
	void from_json(nlohmann::json j, typename space<Tag, Scalar, DimensionCount, Buffer>::point const& arg) {
		for (std::size_t i = 0; i < DimensionCount; ++i) {
			nlohmann::adl_serializer<Scalar>::from_json(j[i], arg[i]);
		}
	}
	template <typename Tag, typename Scalar, int DimensionCount, typename Buffer>
	void to_json(nlohmann::json j, typename space<Tag, Scalar, DimensionCount, Buffer>::point& arg) {
		for (std::size_t i = 0; i < DimensionCount; ++i) {
			nlohmann::adl_serializer<Scalar>::to_json(j[i], arg[i]);
		}
	}

	// vector

	template <typename Tag, typename Scalar, int DimensionCount, typename Buffer>
	void from_json(nlohmann::json j, typename space<Tag, Scalar, DimensionCount, Buffer>::vector const& arg) {
		for (std::size_t i = 0; i < DimensionCount; ++i) {
			nlohmann::adl_serializer<Scalar>::from_json(j[i], arg[i]);
		}
	}
	template <typename Tag, typename Scalar, int DimensionCount, typename Buffer>
	void to_json(nlohmann::json j, typename space<Tag, Scalar, DimensionCount, Buffer>::vector& arg) {
		for (std::size_t i = 0; i < DimensionCount; ++i) {
			nlohmann::adl_serializer<Scalar>::to_json(j[i], arg[i]);
		}
	}

	// box

	template <typename Tag, typename Scalar, int DimensionCount, typename Buffer>
	void from_json(nlohmann::json j, typename space<Tag, Scalar, DimensionCount, Buffer>::box const& arg) {
		from_json(j["position"], arg.position);
		from_json(j["size"], arg.size);
	}
	template <typename Tag, typename Scalar, int DimensionCount, typename Buffer>
	void to_json(nlohmann::json j, typename space<Tag, Scalar, DimensionCount, Buffer>::box& arg) {
		to_json(j["position"], arg.position);
		to_json(j["size"], arg.size);
	}

	// sphere

	template <typename Tag, typename Scalar, int DimensionCount, typename Buffer>
	void from_json(nlohmann::json j, typename space<Tag, Scalar, DimensionCount, Buffer>::sphere const& arg) {
		from_json(j["center"], arg.center);
		from_json(j["radius"], arg.radius);
	}
	template <typename Tag, typename Scalar, int DimensionCount, typename Buffer>
	void to_json(nlohmann::json j, typename space<Tag, Scalar, DimensionCount, Buffer>::sphere& arg) {
		to_json(j["center"], arg.center);
		to_json(j["radius"], arg.radius);
	}
}
