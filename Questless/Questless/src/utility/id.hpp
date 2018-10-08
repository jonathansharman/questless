//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <cstdint>
#include <utility>

// Specialize std::hash.
namespace ql {
	template <typename T>
	struct id;
}
namespace std {
	template <typename T>
	struct hash<ql::id<T>> {
		size_t operator ()(ql::id<T> const& id) const;
	};
}

namespace ql {
	//! Uniquely identifies an object of type T.
	template <typename T>
	struct id {
		using key_t = uint64_t;

		key_t key;

		id() = delete;
		constexpr id(id const&) = default;
		constexpr explicit id(key_t key) : key{key} {}

		constexpr id& operator =(id const&) = default;

		//! A new unique ID.
		static id make() {
			//! @todo This won't work once saving and loading are in place.
			static key_t next_key = 0;
			return id{next_key++};
		}

		template <typename Archive>
		key_t save_minimal(Archive const&) const { return key; }

		template <typename Archive>
		void load_minimal(Archive const&, key_t const& key) { this->key = key; }

		constexpr bool operator <(id const& that) const { return key < that.key; }
		constexpr bool operator <=(id const& that) const { return key <= that.key; }
		constexpr bool operator ==(id const& that) const { return key == that.key; }
		constexpr bool operator !=(id const& that) const { return key != that.key; }
		constexpr bool operator >(id const& that) const { return key > that.key; }
		constexpr bool operator >=(id const& that) const { return key >= that.key; }
	};
}

// Define std::hash specialization.
namespace std {
	template <typename T>
	size_t hash<ql::id<T>>::operator ()(ql::id<T> const& id) const {
		return hash<typename ql::id<T>::key_t>{}(id.key);
	}
}
