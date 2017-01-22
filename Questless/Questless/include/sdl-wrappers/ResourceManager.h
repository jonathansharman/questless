/**
* @file    ResourceManager.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the ResourceManager class template, which manages shared resources of a given type.
*/

#pragma once

#include <string>
#include <list>
#include <functional>
#include <memory>

namespace sdl
{
	template <typename ResourceType>
	struct Entry
	{
		Entry(std::unique_ptr<ResourceType> resource, std::function<std::unique_ptr<ResourceType>()> generator)
			: resource{std::move(resource)}, generator{std::move(generator)}
		{}

		mutable std::unique_ptr<ResourceType> resource; // Mutable to support lazy evaluation.
		std::function<std::unique_ptr<ResourceType>()> generator;
	};

	template <typename ResourceType>
	struct Handle
	{
		typename std::list<Entry<ResourceType>>::iterator it;
	};

	template <typename ResourceType>
	class ResourceManager
	{
	public:
		/// Registers the given resource name and generator with the manager.
		/// @param generator A function that loads or creates the resource when it is needed.
		/// @return The handle with which the resource can be accessed.
		Handle<ResourceType> add(std::function<std::unique_ptr<ResourceType>()> generator)
		{
			_registry.emplace_front(nullptr, std::move(generator));
			return Handle<ResourceType>{_registry.begin()};
		}

		/// @param handle The handle of the desired resource, obtained from the initial call to add().
		/// @return The resource at the given index.
		ResourceType& operator [](Handle<ResourceType> const& handle) const
		{
			auto& entry = *handle.it;
			if (entry.resource == nullptr) {
				entry.resource = entry.generator();
			}
			return *entry.resource;
		}

		/// Removes the given resource and its generator from the registry.
		/// @param handle The handle of the resource to be removed, obtained from the initial call to add().
		void erase(Handle<ResourceType> const& handle)
		{
			_registry.erase(handle._it);
		}

		/// Removes the given resources and their generators from the registry.
		/// @param names The names of the resources to be removed.
		void erase(std::vector<Handle<ResourceType>> const& handles)
		{
			for (Handle const& handle : handles) {
				erase(handle);
			}
		}

		/// Clears the cache such that each resource must be regenerated when next accessed.
		void clear_cache()
		{
			for (auto& entry : _registry) {
				entry.resource = nullptr;
			}
		}
	private:
		std::list<Entry<ResourceType>> _registry;
	};
}
