//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <deque>

#include "animation.hpp"
#include "particles/particle.hpp"
#include "utility/reference.hpp"

namespace ql {
	//! Abstract base for animations composed of particle effects.
	class particle_animation : public animation {
	public:
		std::deque<uptr<particle>> particles;

		virtual ~particle_animation() = default;
	private:
		void animation_subupdate(sec elapsed_time) final;

		void animation_subdraw(sf::RenderTarget& target, sf::RenderStates states) const final;

		//! Advances this particle animation by @p elapsed_time in a subtype-specific way.
		virtual void particle_animation_subupdate(sec elapsed_time) = 0;
	};
}
