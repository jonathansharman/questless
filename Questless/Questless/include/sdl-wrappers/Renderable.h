/**
* @file    Renderable.h
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The interface for the Renderable abstract base class. Includes texture and font managers for shared resources.
*/

#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <set>

#include "sdl-wrappers/resources.h" // Included for convenience since virtually all Renderables need access to shared resources.

namespace sdl
{
	class Renderable
	{
	public:
		/// Refreshes all renderable objects.
		static void refresh_all();
	protected:
		Renderable() { _registry.insert(this); }
		~Renderable() { _registry.erase(this); }
	private:
		static std::set<Renderable*> _registry;

		virtual void refresh() = 0;
	};
}

#endif
