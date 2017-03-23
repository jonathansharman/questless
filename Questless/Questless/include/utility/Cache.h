/**
* @file    Cache.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*/

#pragma once

#include <unordered_map>
#include <memory>

#include "Id.h"

namespace questless
{
	/// Stores objects of the given type and manages their transfer to/from disk.
	////
	template <typename ElementType>
	class Cache
	{
	public:
		using element_t = ElementType;

		/// Adds the given object to the cache. If the object's ID is not unique, may overwrite an existing object.
		/// @param object The object to be added.
		/// @return A reference to the added object.
		////
		element_t& add(std::unique_ptr<element_t> object)
		{
			element_t& object_ref = *object;
			_cache[object->id] = std::move(object);
			return object_ref;
		}

		/// Retrieves an object from the cache, loading it from disk if necessary.
		/// @param id The ID of the desired object.
		/// @return A pointer to the requested object or nullptr if the object does not exist.
		////
		element_t* get(Id<element_t> id) const
		{
			auto it = _cache.find(id);
			if (it != _cache.end()) {
				return it->second.get();
			} else {
				// Object is not in the cache. Attempt to load from disk.

				/// @todo This.
				throw std::logic_error{"Load from disk not yet supported."};
			}
		}

		/// Retrieves an object from the cache, loading it from disk if necessary, and dynamically casts it to a pointer of the given type.
		/// @param id The ID of the desired object.
		/// @tparam TargetType The desired type of the returned element. Must be a base class or derived class of the cache element type.
		/// @return A pointer to the requested object as the desired type or nullptr if the object does not exist.
		////
		template <typename TargetType>
		TargetType* get_as(Id<element_t> id) const
		{
			constexpr bool valid_cast = std::is_base_of<element_t, TargetType>::value || std::is_base_of<TargetType, element_t>::value;
			static_assert(valid_cast, "Cache::get_as requires \"is-a\" relationship between the cache element type and the target type.");

			return dynamic_cast<TargetType*>(get(id));
		}


		/// Removes the object with the given ID from the cache, if present.
		/// @param id The ID of the object to be removed.
		////
		void erase(Id<element_t> id) { _cache.erase(id); }

		/// Removes all objects from the cache.
		////
		void clear() { _cache.clear(); }
	private:
		std::unordered_map<Id<element_t>, std::unique_ptr<element_t>> _cache;
	};
}
