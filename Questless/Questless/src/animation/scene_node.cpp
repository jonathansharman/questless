//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "scene_node.hpp"

#include <range/v3/action/remove_if.hpp>
#include <range/v3/view/reverse.hpp>

namespace ql {
	scene_node::scene_node(uptr<animation> node_animation) : node_animation{std::move(node_animation)} {}

	auto scene_node::animation_subupdate(sec elapsed_time) -> void {
		// Update this node's animation.
		if (node_animation) {
			node_animation->update(elapsed_time);
			if (node_animation->stopped()) {
				stop();
				return;
			}
		}

		// Update child animations, factoring in this node's time scale.
		auto update_children = [&](auto& children) {
			for (auto& child : children) {
				child->update(elapsed_time * node_animation->time_scale.value());
			}
			// Remove stopped children.
			ranges::actions::remove_if(children, [](auto& child) { return child->stopped(); });
		};
		update_children(back_children);
		update_children(front_children);
	}

	auto scene_node::animation_subdraw(sf::RenderTarget& target, sf::RenderStates states) const -> void {
		for (auto& child : back_children | ranges::views::reverse) {
			target.draw(*child, states);
		}
		if (node_animation) { target.draw(*node_animation, states); }
		for (auto& child : front_children | ranges::views::reverse) {
			target.draw(*child, states);
		}
	}
}
