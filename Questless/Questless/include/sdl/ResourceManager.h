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
	template <typename ResourceType>
	class ResourceManager;

	namespace detail
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
	}

	//! Opaque handle to a resource of type @p ResourceType.
	template <typename ResourceType>
	class Handle
	{
	public:
		using resource_t = ResourceType;

		constexpr Handle() = default;
		constexpr Handle(Handle const&) = default;
		constexpr Handle(Handle&&) = default;
		Handle& operator =(Handle const&) = default;
		Handle& operator =(Handle&&) = default;

		constexpr bool operator ==(Handle const& that) const { return it == that.it; }
		constexpr bool operator !=(Handle const& that) const { return it != that.it; }

		//! Hash based on address of referenced resource.
		friend std::size_t hash_value(Handle const& handle)
		{
			return std::hash<decltype(&*handle.it)>{}(&*handle.it);
		}
	private:
		template <typename> friend class ResourceManager;

		using it_t = typename std::list<detail::Entry<resource_t>>::iterator;
		it_t it;

		Handle(it_t it) : it{it} {}
	};

	//! Manages shared resources of a given type.
	template <typename ResourceType>
	class ResourceManager
	{
	public:
		using resource_t = ResourceType;
		using handle_t = Handle<resource_t>;

		//! Registers with the manager a resource constructed from the given constructor arguments.
		//! @param args Arguments with which to construct the resource in the internal generator.
		//! @return The handle with which the resource can be accessed.
		template <typename... Args>
		handle_t add(Args... args)
		{
			_registry.emplace_front(nullptr, [args...] {
				return std::make_unique<ResourceType>(args...);
			});
			return handle_t{_registry.begin()};
		}

		//! @todo Enable the use of the following specialization without the need for explicit parameterization.

		//! Registers with the manager a resource with the given generator.
		//! @param generator A function that loads or creates the resource when it is needed.
		//! @return The handle with which the resource can be accessed.
		handle_t add_with_generator(std::function<std::unique_ptr<ResourceType>()> generator)
		{
			_registry.emplace_front(nullptr, std::move(generator));
			return handle_t{_registry.begin()};
		}

		//! @param handle The handle of the desired resource, obtained from the initial call to add().
		//! @return The resource with the given handle.
		ResourceType& operator [](handle_t const& handle) const
		{
			auto& entry = *handle.it;
			if (entry.resource == nullptr) {
				entry.resource = entry.generator();
			}
			return *entry.resource;
		}

		//! Removes the given resource and its generator from the registry.
		//! @param handle The handle of the resource to be removed, obtained from the initial call to add().
		void erase(handle_t const& handle)
		{
			_registry.erase(handle._it);
		}

		//! Removes the given resources and their generators from the registry.
		//! @param names The names of the resources to be removed.
		void erase(std::vector<handle_t> const& handles)
		{
			for (handle_t const& handle : handles) {
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
		std::list<detail::Entry<resource_t>> _registry;
	};
}

// Specialize std::hash.
namespace std
{
	template <typename ResourceType>
	struct hash<sdl::Handle<ResourceType>>
	{
		size_t operator()(sdl::Handle<ResourceType> const& handle) const
		{
			return hash_value(handle);
		}
	};
}
