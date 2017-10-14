//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <cstdint>

// Specialize std::hash.
namespace ql
{
	template <typename T>
	class id;
}
namespace std
{
	template <typename T>
	struct hash<ql::id<T>>
	{
		size_t operator ()(ql::id<T> const& id) const;
	};
}

namespace ql
{
	//! Uniquely identifies an object of type T.
	template <typename T>
	class id
	{
	public:
		using key_t = uint64_t;

		id() = delete;
		id(id const&) = default;
		constexpr explicit id(key_t key) : _key{key} {}

		//! Constructs an ID from a key read from the stream.
		id(std::istream& in) : _key{read_key(in)} {}

		id& operator =(id const&) & = default;

		//! A new unique ID.
		static id make() { return id{next()}; }

		bool operator <(id that) const { return _key < that._key; }
		bool operator ==(id that) const { return _key == that._key; }

		//! Inserts the ID's key into the stream.
		friend std::ostream& operator <<(std::ostream& out, id id)
		{
			out << id._key;
			return out;
		}
	private:
		key_t _key;
		
		friend size_t std::hash<id>::operator ()(const id&) const;

		//! The next unused ID key.
		static key_t next()
		{
			//! @todo This won't work once saving and loading are in place.
			static key_t next_key = 0;
			return next_key++;
		}

		//! Returns an ID key read from the given stream.
		static key_t read_key(std::istream& in)
		{
			key_t key;
			in >> key;
			return key;
		}
	};
}

// Define std::hash specialization.
namespace std
{
	template <typename T>
	size_t hash<ql::id<T>>::operator ()(ql::id<T> const& id) const
	{
		return hash<ql::id<T>::key_t>{}(id._key);
	}
}
