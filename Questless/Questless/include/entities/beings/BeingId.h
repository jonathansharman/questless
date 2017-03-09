/**
* @file    BeingId.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The definition of the BeingId type, used to uniquely identify beings.
*/

#pragma once

#include <cstdint>

namespace questless
{
	struct BeingId
	{
		using key_t = uint64_t;

		static BeingId next()
		{
			/// @todo This is bad.
			static key_t next_key = 0;
			return next_key++;
		}

		key_t key;

		BeingId() : key{0} {}
		BeingId(key_t key) : key{key} {}

		bool operator <(BeingId other) const { return key < other.key; }
		bool operator ==(BeingId other) const { return key == other.key; }
	};
}

// Specialize std::hash.
namespace std
{
	template <>
	struct hash<questless::BeingId>
	{
		size_t operator()(questless::BeingId const& id) const
		{
			return hash<questless::BeingId::key_t>{}(id.key);
		}
	};
}
