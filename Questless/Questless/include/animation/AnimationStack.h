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

		void draw(units::ScreenSpace::Point position) const
		{
			for (auto& animation : _animations) {
				animation->draw(position);
			}
		}

		void draw(units::GameSpace::Point position, Camera const& camera, units::colors::ColorFactor color_factor = units::colors::white_factor()) const
		{
			for (auto& animation : _animations) {
				animation->draw(position, camera, color_factor);
			}
		}
	private:
		std::vector<uptr<Animation>> _animations;

		void animation_subupdate() final
		{
			for (auto& animation : _animations) {
				animation->update();
			}
		}
	};
}
