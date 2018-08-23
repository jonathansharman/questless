//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "animation/body_texturer.hpp"

#include <unordered_map>

#include "entities/beings/body.hpp"
#include "entities/beings/body_part.hpp"
#include "sdl/resources.hpp"
#include "utility/utility.hpp"

using namespace sdl;
using namespace units;

namespace {
	colors::color get_color(ql::body_part const& part) {
		double const pct_health = part.health / part.vitality;
		constexpr double threshold = 0.5f;
		bool const below_threshold = pct_health < threshold;
		float const red = below_threshold
			? 1.0f
			: static_cast<float>(1.0 - (pct_health - threshold) / (1.0 - threshold))
			;
		float const green = below_threshold
			? static_cast<float>(pct_health / threshold)
			: 1.0f
			;
		return colors::color{red, green, 0.0f, 1.0f};
	};

	struct render_data {
		std::unordered_multimap<int, std::tuple<view_space::polygon, colors::color>> colored_hitboxes;
		view_space::box bounds;
	};

	render_data get_render_data(ql::body_part const& root) {
		std::unordered_multimap<int, std::tuple<view_space::polygon, colors::color>> colored_hitboxes;

		// Initialize bounds to just the first vertex of the root part.
		view_space::box bounds{root.hitbox().front(), view_space::vector::zero()};

		std::function<void(ql::body_part const&, view_space::vector, view_space::radians)>
		traverse = [&](ql::body_part const& part, view_space::vector parent_offset, view_space::radians parent_rotation) {
			// Adjust hitbox and extend body bounds.
			auto hitbox = part.hitbox();
			for (auto& vertex : hitbox) {
				vertex.rotate(view_space::point{0.0f, 0.0f}, parent_rotation);
				vertex += parent_offset;
				bounds.extend(vertex);
			}

			// Color hitbox and add to layer-sorted map.
			colored_hitboxes.insert(std::make_pair(part.layer(), std::make_tuple(hitbox, get_color(part))));
			for (auto const& attachment : part.attachments()) {
				if (attachment->part) {
					traverse
						( *attachment->part
						, attachment->offset().rotated(parent_rotation) + parent_offset
						, attachment->rotation() + parent_rotation
						);
				}
			}
		};
		traverse(root, view_space::vector::zero(), view_space::radians::zero());

		return render_data{std::move(colored_hitboxes), std::move(bounds)};
	}
}

namespace ql {
	void body_texturer::visit(body const& body) {
		render_data data = get_render_data(body.root());

		window_space::vector const texture_size
			{ static_cast<int>(data.bounds.size.x())
			, static_cast<int>(data.bounds.size.y())
			};
		colors::color const texture_color{0.0f, 1.0f, 1.0f, 0.5f};
		_texture = umake<sdl::texture>(texture_size, texture_color);
		_texture->as_target([&] {
			auto offset_to_center = view_space::point{0.0f, 0.0f} - data.bounds.position;

			{ // Draw outline.
				window_space::box outline
					{ window_space::point{0, 0}
					, window_space::vector{_texture->width(), _texture->height()}
					};
				the_renderer().draw_box(outline, 1, colors::red(), colors::clear());
			}

			for (auto& kv : data.colored_hitboxes) {
				auto [hitbox, color] = kv.second;
				for (auto& vertex : hitbox) {
					vertex += offset_to_center;
				}
				the_renderer().draw_polygon(hitbox, 2, colors::black(), color);
			}
		}, solid_program());
	}
}
