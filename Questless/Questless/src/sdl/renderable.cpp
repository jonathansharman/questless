//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "sdl/renderable.hpp"

namespace sdl
{
	std::unordered_set<renderable*> renderable::_registry;

	void renderable::refresh_all()
	{
		for (renderable* renderable : _registry) {
			renderable->refresh();
		}
	}
}
