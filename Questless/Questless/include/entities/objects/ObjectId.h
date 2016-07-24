/**
* @file    ObjectId.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The definition of the BeingId type, used to uniquely identify beings.
*/

#ifndef OBJECT_ID_H
#define OBJECT_ID_H

namespace questless
{
	struct ObjectId
	{
		using key_t = unsigned long long;

		static ObjectId next()
		{
			/// @todo This is bad.
			static key_t next_key = 0;
			return next_key++;
		}

		key_t key;

		ObjectId() : key{0} {}
		ObjectId(key_t key) : key{key} {}

		bool operator <(ObjectId other) const { return key < other.key; }
		bool operator ==(ObjectId other) const { return key == other.key; }
	};
}

// Specialize std::hash.
namespace std
{
	template <>
	struct hash<questless::ObjectId>
	{
		size_t operator()(const questless::ObjectId& id) const
		{
			return hash<questless::ObjectId::key_t>{}(id.key);
		}
	};
}

#endif
