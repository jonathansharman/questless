/**
* @file    Renderable.cpp
* @author  Jonathan Sharman
*
* @section LICENSE See LICENSE.txt.
*
* @section DESCRIPTION The implementation for the Renderable abstract base class.
*/

#include "sdl-wrappers/Renderable.h"

namespace sdl
{
	std::unordered_set<Renderable*> Renderable::_registry;

	void Renderable::refresh_all()
	{
		for (Renderable* renderable : _registry) {
			renderable->refresh();
		}
	}
}
