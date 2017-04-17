//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "sdl/Renderable.h"

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
