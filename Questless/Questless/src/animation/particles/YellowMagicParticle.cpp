//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/particles/YellowMagicParticle.h"
#include "sdl/resources.h"

namespace questless
{
	void YellowMagicParticle::subupdate()
	{
		_velocity.step().rotate(uniform(-1.0, 1.0) * _max_turn_rate);
		_angle = _velocity.step().angle();
	}

	sdl::Texture const& YellowMagicParticle::texture() const
	{
		static auto texture_handle = sdl::texture_manager().add("resources/textures/particles/magic/yellow.png");
		return sdl::texture_manager()[texture_handle];
	}
}
