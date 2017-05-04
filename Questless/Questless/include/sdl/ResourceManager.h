//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <string>
#include <vector>
#include <list>
#include <functional>
#include <memory>

namespace sdl
{
	//! Manages shared resources of a given type.
	template <typename ResourceType>
	class ResourceManager
	{
	public:
		struct Entry
		{
			Entry(std::unique_ptr<ResourceType> resource, std::function<std::unique_ptr<ResourceType>()> generator)
				: resource{std::move(resource)}, generator{std::move(generator)}
			{}

			mutable std::unique_ptr<ResourceType> resource; // Mutable to support lazy evaluation.
			std::function<std::unique_ptr<ResourceType>()> generator;
		};

		class Handle
		{
		public:
			Handle() = default;
			Handle(Handle const&) = default;
			Handle(Handle&&) = default;
			Handle& operator =(Handle const&) = default;
			Handle& operator =(Handle&&) = default;
		private:
			friend class ResourceManager;
			typename std::list<Entry>::iterator it;

			Handle(decltype(it) it) : it{it} {}
		};

		//! Registers with the manager a resource constructed from the given arguments.
		//! @param args Arguments with which to construct the resource in the generator.
		//! @return The handle with which the resource can be accessed.
		template <typename... Args>
		Handle add(Args... args)
		{
			_registry.emplace_front(nullptr, [args...] {
				return std::make_unique<ResourceType>(args...);
			});
			return Handle{_registry.begin()};
		}

		//! @todo Enable the use of the following specialization without the need for explicit parameterization.

		//! Registers with the manager a resource with the given generator.
		//! @param generator A function that loads or creates the resource when it is needed.
		//! @return The handle with which the resource can be accessed.
		template <>
		Handle add(std::function<std::unique_ptr<ResourceType>()> generator)
		{
			_registry.emplace_front(nullptr, std::move(generator));
			return Handle{_registry.begin()};
		}

		//! @param handle The handle of the desired resource, obtained from the initial call to add().
		//! @return The resource with the given handle.
		ResourceType& operator [](Handle const& handle) const
		{
			auto& entry = *handle.it;
			if (entry.resource == nullptr) {
				entry.resource = entry.generator();
			}
			return *entry.resource;
		}

		//! Removes the given resource and its generator from the registry.
		//! @param handle The handle of the resource to be removed, obtained from the initial call to add().
		void erase(Handle const& handle)
		{
			_registry.erase(handle._it);
		}

		//! Removes the given resources and their generators from the registry.
		//! @param names The names of the resources to be removed.
		void erase(std::vector<Handle> const& handles)
		{
			for (Handle const& handle : handles) {
				erase(handle);
			}
		}

		//! Clears the cache such that each resource must be regenerated when next accessed.
		void clear_cache()
		{
			for (auto& entry : _registry) {
				entry.resource = nullptr;
			}
		}
	private:
		std::list<Entry> _registry;
	};
}
