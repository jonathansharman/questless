//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.
//! @brief Common utility functions for loading resources.

#pragma once

namespace ql::rsrc {
	//! Loads a resource from a file. Useful for loading a texture in one line.
	//! @p path The path to the file that holds the resource.
	template <typename ResourceType>
	auto load(char const* path) {
		ResourceType result;
		result.loadFromFile(path);
		return result;
	}
}
