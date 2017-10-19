//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <cassert>
#include <unordered_map>

#include "id.hpp"
#include "reference.hpp"

namespace ql
{
	//! Stores elements of type @p StoredType with ID type @IdType and manages their transfer to/from disk.
	//! @tparam IdType The type that uniquely identifies elements.
	//! @tparam StoredType The type of elements stored in the cache.
	//! @tparam RetrievedType The type of elements retrieved from the cache.
	//! @tparam id_to_element Mapping function from elements to their unique IDs.
	//! @tparam stored_to_retrieved_ptr Mapping function from stored element to pointer to retrieved element.
	template
		< typename IdType
		, typename StoredType
		, typename RetrievedType
		, IdType (*id_to_element)(StoredType const&)
		, RetrievedType* (*stored_to_retrieved_ptr)(StoredType const&)
		>
	class cache
	{
	public:
		using id_t = IdType;
		using stored_t = StoredType;
		using retrieved_t = RetrievedType;

		//! Adds @p element to the cache. If the element's ID is not unique, this may overwrite an existing element.
		//! @return A reference to the added object.
		retrieved_t& add(stored_t element)
		{
			retrieved_t* retrieved = stored_to_retrieved_ptr(element);
			id_t id = id_to_element(element);
			_cache.insert(std::make_pair(id, std::move(element)));
			return *retrieved;
		}

		//! Retrieves an element from the cache, loading it from disk if necessary.
		//! @param id The ID of the desired element.
		//! @return A pointer to the requested element or nullptr if it does not exist.
		retrieved_t* ptr(id_t id)
		{
			auto it = _cache.find(id);
			if (it != _cache.end()) {
				return stored_to_retrieved_ptr(it->second);
			} else {
				// Element is not in the cache. Attempt to load from disk.

				//! @todo This.
				throw std::logic_error{"Element not found. (Loading from disk not yet supported.)"};
			}
		}
		//! Retrieves an element from the cache, loading it from disk if necessary.
		//! @param id The ID of the desired element.
		//! @return A const pointer to the requested element or nullptr if it does not exist.
		retrieved_t const* cptr(id_t id) const
		{
			auto it = _cache.find(id);
			if (it != _cache.end()) {
				return stored_to_retrieved_ptr(it->second);
			} else {
				// Element is not in the cache. Attempt to load from disk.

				//! @todo This.
				throw std::logic_error{"Element not found. (Loading from disk not yet supported.)"};
			}
		}

		//! Retrieves an element from the cache, loading it from disk if necessary, and dynamically casts it to a pointer of the given type.
		//! @param id The ID of the desired element.
		//! @tparam TargetType The desired type of the returned element. Must be a base class or derived class of the cache element type.
		//! @return A pointer to the requested element as the desired type or nullptr if the element does not exist.
		template <typename TargetType>
		TargetType* ptr_as(id_t id)
		{
			constexpr bool valid_cast = std::is_base_of<retrieved_t, TargetType>::value || std::is_base_of<TargetType, retrieved_t>::value;
			static_assert(valid_cast, "cache::get_as requires \"is-a\" relationship between the cache element type and the target type.");

			return dynamic_cast<TargetType*>(ptr(id));
		}
		//! Retrieves an element from the cache, loading it from disk if necessary, and dynamically casts it to a pointer of the given type.
		//! @param id The ID of the desired element.
		//! @tparam TargetType The desired type of the returned element. Must be a base class or derived class of the cache element type.
		//! @return A const pointer to the requested element as the desired type or nullptr if the element does not exist.
		template <typename TargetType>
		TargetType const* cptr_as(id_t id) const
		{
			constexpr bool valid_cast = std::is_base_of<retrieved_t, TargetType>::value || std::is_base_of<TargetType, retrieved_t>::value;
			static_assert(valid_cast, "cache::get_as requires \"is-a\" relationship between the cache element type and the target type.");

			return dynamic_cast<TargetType const*>(cptr(id));
		}

		//! Retrieves an element from the cache, loading it from disk if necessary.
		//! @param id The ID of the desired element.
		//! @return A reference to the requested element.
		//! @note The element must exist, or this will fail.
		retrieved_t& ref(id_t id)
		{
			retrieved_t* result_ptr = ptr(id);
			assert(result_ptr);
			return *result_ptr;
		}
		//! Retrieves an element from the cache, loading it from disk if necessary.
		//! @param id The ID of the desired element.
		//! @return A const reference to the requested element.
		//! @note The element must exist, or this will fail.
		retrieved_t const& cref(id_t id) const
		{
			retrieved_t const* result_cptr = cptr(id);
			assert(result_cptr);
			return *result_cptr;
		}

		//! Retrieves an element from the cache, loading it from disk if necessary, and dynamically casts it to a reference of the given type.
		//! @param id The ID of the desired element.
		//! @tparam TargetType The desired type of the returned element. Must be a base class or derived class of the cache element type.
		//! @return A reference to the requested element as the desired type.
		//! @note The element must exist, or this will fail.
		template <typename TargetType>
		TargetType& ref_as(id_t id) const
		{
			TargetType* result_ptr = ptr_as<TargetType>(id);
			assert(result_ptr);
			return *result_ptr;
		}
		//! Retrieves an element from the cache, loading it from disk if necessary, and dynamically casts it to a reference of the given type.
		//! @param id The ID of the desired element.
		//! @tparam TargetType The desired type of the returned element. Must be a base class or derived class of the cache element type.
		//! @return A reference to the requested element as the desired type.
		//! @note The element must exist, or this will fail.
		template <typename TargetType>
		TargetType const& cref_as(id_t id) const
		{
			TargetType const* result_cptr = cptr_as<TargetType>(id);
			assert(result_cptr);
			return *result_cptr;
		}

		//! Removes the element with ID @id from the cache, if present.
		void erase(id_t id) { _cache.erase(id); }

		//! Removes all elements from the cache.
		void clear() { _cache.clear(); }
	private:
		std::unordered_map<id_t, stored_t> _cache;
	};
}
