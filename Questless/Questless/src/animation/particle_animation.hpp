//! @file
//! @copyright See <a href="LICENSE.txt">LICENSE.txt</a>.

#pragma once

#include "animation.hpp"
#include "particles/particle.hpp"

#include "utility/reference.hpp"

#include <deque>

namespace ql {
	//! Abstract base for animations composed of particle effects.
	struct particle_animation : animation {
		std::deque<uptr<particle>> particles;

		virtual ~particle_animation() = default;

	private:
		auto animation_subupdate(sec elapsed_time) -> void final;

		auto animation_subdraw(sf::RenderTarget& target, sf::RenderStates states) const -> void final;

		//! Advances this particle animation by @p elapsed_time in a subtype-specific way.
		virtual auto particle_animation_subupdate(sec elapsed_time) -> void = 0;
	};
}
