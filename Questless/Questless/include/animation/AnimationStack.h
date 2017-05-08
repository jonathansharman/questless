//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include <optional>
#include <vector>

#include "Animation.h"
#include "utility/reference.h"

namespace questless
{
	//! A stack of associated animations. Useful for multi-animation effects, such as a sprite animation combined with a particle animation.
	//!
	//! @note Animations are updated and drawn in the same order they were added to the set.
	//!
	class AnimationStack : public Animation
	{
	public:
		//! Adds @p animation to the set.
		//! @return A reference to @p animation.
		Animation& add(uptr<Animation> animation)
		{
			_animations.push_back(std::move(animation));
			return *_animations.back();
		}

		void draw(units::ScreenPoint position) const
		{
			for (auto& animation : _animations) {
				animation->draw(position);
			}
		}

		void draw(units::GamePoint position, Camera const& camera, sdl::Color color = sdl::Color::white()) const
		{
			for (auto& animation : _animations) {
				animation->draw(position, camera, color);
			}
		}
	protected:
		void animation_subupdate() final
		{
			for (auto& animation : _animations) {
				animation->update();
			}
		}
	private:
		std::vector<uptr<Animation>> _animations;
	};
}
