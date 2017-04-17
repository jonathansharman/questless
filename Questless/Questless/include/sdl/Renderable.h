//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <unordered_set>

#include "sdl/resources.h" // Included for convenience since virtually all Renderables need access to shared resources.

namespace sdl
{
	//! An object that owns textures, which must be refreshed whenever the renderer is remade.
	//!
	//! Inherit from Renderable to refresh textures automatically whenever Renderable::refresh_all() is called.
	class Renderable
	{
	public:
		//! Refreshes all renderable objects, reloading or remaking their held textures.
		//!
		//! This function should always be called after the window and renderer are remade, such as after resizing the window.
		static void refresh_all();
	protected:
		Renderable() { _registry.insert(this); }
		~Renderable() { _registry.erase(this); }
	private:
		static std::unordered_set<Renderable*> _registry;

		//! Reloads or remakes any textures owned by this object.
		//!
		virtual void refresh() = 0;
	};
}
