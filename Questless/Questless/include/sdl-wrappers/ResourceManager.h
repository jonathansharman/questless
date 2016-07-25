/**
* @file    ResourceManager.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the ResourceManager class template, which manages shared resources of a given type.
*/

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <exception>
#include <memory>

/// @todo The resource manager is quite inefficient. Filling the screen with particles that use the texture manager causes the FPS to plummet.
///       Need to do away with the string-based keys.

namespace sdl
{
	template <typename ResourceType>
	class ResourceManager
	{
	public:
		/// Registers the given resource name and generator with the manager.
		/// @param name The name by which the resource should be accessed.
		/// @param generator A function that loads or creates the resource when it is needed.
		void add(std::string name, std::function<std::unique_ptr<ResourceType>()> generator) { _registry[name] = make_pair(nullptr, std::move(generator)); }

		/// @return The resource with the given name.
		/// @param name The name of the resource to be retrieved.
		ResourceType& get(std::string name)
		{
			auto it = _registry.find(name);
			if (it == _registry.end()) {
				throw std::logic_error{'\"' + name + "\" not found in resource manager."};
			} else {
				auto& entry = it->second;
				if (entry.first == nullptr) {
					entry.first = entry.second();
				}
				return *entry.first;
			}
		}

		/// @return The resource with the given name.
		/// @param name The name of the resource to be retrieved.
		ResourceType& operator [](std::string name) { return get(name); }

		/// Removes the given resource and its generator from the registry.
		/// @param name The name of the resource to be removed.
		void erase(std::string name) { _registry.erase(name); }

		/// Removes the given resources and their generators from the registry.
		/// @param names The names of the resources to be removed.
		void erase(std::vector<std::string> names)
		{
			for (std::string name : names) {
				erase(name);
			}
		}

		/// Clears the cache such that each resource must be regenerated when next accessed.
		void clear_cache()
		{
			for (auto& name_and_entry : _registry) {
				name_and_entry.second.first = nullptr;
			}
		}
	private:
		std::unordered_map<std::string, std::pair<std::unique_ptr<ResourceType>, std::function<std::unique_ptr<ResourceType>()>>> _registry;
	};
}

#endif
