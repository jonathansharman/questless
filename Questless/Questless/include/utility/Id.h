/**
* @file    Id.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*/

#pragma once

#include <cstdint>

// Specialize std::hash.
namespace questless
{
	template <typename T>
	class Id;
}
namespace std
{
	template <typename T>
	struct hash<questless::Id<T>>
	{
		size_t operator ()(questless::Id<T> const& id) const;
	};
}

namespace questless
{
	/// Uniquely identifies an object of type T.
	////
	template <typename T>
	class Id
	{
	public:
		using key_t = uint64_t;

		Id() = delete;
		Id(Id const&) = default;
		constexpr explicit Id(key_t key) : _key{key} {}

		/// Constructs an ID from a key read from the stream.
		////
		Id(std::istream& in) : _key{read_key(in)} {}

		Id& operator =(Id const&) & = default;

		/// @return A new unique ID.
		////
		static Id make() { return Id{next()}; }

		bool operator <(Id that) const { return _key < that._key; }
		bool operator ==(Id that) const { return _key == that._key; }

		/// Inserts the ID's key into the stream.
		////
		friend std::ostream& operator <<(std::ostream& out, Id id)
		{
			out << id._key;
			return out;
		}
	private:
		key_t _key;
		
		friend size_t std::hash<Id>::operator ()(const Id&) const;

		/// @return The next unused ID key.
		////
		static key_t next()
		{
			/// @todo This won't work once saving and loading are in place.
			static key_t next_key = 0;
			return next_key++;
		}

		/// Returns an ID key read from the given stream.
		////
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
	size_t hash<questless::Id<T>>::operator ()(questless::Id<T> const& id) const
	{
		return hash<questless::Id<T>::key_t>{}(id._key);
	}
}
