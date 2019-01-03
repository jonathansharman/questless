//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "body_texturer.hpp"

#include "entities/beings/body.hpp"
#include "entities/beings/body_part.hpp"
#include "sdl/resources.hpp"
#include "utility/nonnegative.hpp"
#include "utility/utility.hpp"

#include <unordered_map>

using namespace sdl;
using namespace sdl::spaces;
using namespace sdl::spaces::colors::literals;
using namespace sdl::spaces::view::literals;
using namespace sdl::spaces::window::literals;
using namespace vecx;
using namespace vecx::literals;

namespace ql {
	namespace {
		colors::color get_color(ql::body_part const& part) {
			float const pct_health = static_cast<float>((part.health.value() / part.stats.a.vitality.value()).value);
			constexpr float threshold = 0.5f;
			bool const below_threshold = pct_health < threshold;
			auto const red = below_threshold
				? 1.0_c
				: 1.0_c - sdl::spaces::colors::part{(pct_health - threshold) / (1.0f - threshold)}
				;
			auto const green = below_threshold
				? sdl::spaces::colors::part{pct_health / threshold}
				: 1.0_c
				;
			return colors::color{red, green, 0.0_c, 1.0_c};
		};

		struct render_data {
			std::unordered_multimap<int, std::tuple<view::polygon, colors::color>> colored_hitboxes;
			view::box bounds;
		};

		render_data get_render_data(ql::body_part const& root) {
			std::unordered_multimap<int, std::tuple<view::polygon, colors::color>> colored_hitboxes;

			// Initialize bounds to just the first vertex of the root part.
			view::box bounds{root.hitbox.vertices.front(), view::vector::zero()};

			std::function<void(ql::body_part const&, view::vector, radians)>
			traverse = [&](ql::body_part const& part, view::vector parent_offset, radians parent_rotation) {
				// Adjust hitbox and extend body bounds.
				auto hitbox = part.hitbox;
				for (auto& vertex : hitbox.vertices) {
					vertex.rotate(view::point{0.0_view_length, 0.0_view_length}, parent_rotation);
					vertex += parent_offset;
					bounds.extend(vertex);
				}

				// Color hitbox and add to layer-sorted map.
				colored_hitboxes.insert(std::make_pair(part.layer, std::make_tuple(hitbox, get_color(part))));
				for (auto const& attachment : part.attachments) {
					if (attachment->part) {
						traverse
							( *attachment->part
							, attachment->offset.rotated(parent_rotation) + parent_offset
							, attachment->rotation + parent_rotation
							);
					}
				}
			};
			traverse(root, view::vector::zero(), 0.0_rad);

			return render_data{std::move(colored_hitboxes), std::move(bounds)};
		}
	}

	void body_texturer::visit(body const& body) {
		render_data data = get_render_data(body.root());

		spaces::window::vector const texture_size
			{ spaces::window::px{lround(x(data.bounds.size).value)}
			, spaces::window::px{lround(y(data.bounds.size).value)}
			};
		auto const texture_color = colors::color{0.0_c, 1.0_c, 1.0_c, 0.5_c};
		_texture = umake<sdl::texture>(texture_size, texture_color);
		_texture->as_target([&] {
			auto offset_to_center = view::point{0.0_view_length, 0.0_view_length} - data.bounds.position;

			{ // Draw outline.
				spaces::window::box outline
					{ spaces::window::point{0_px, 0_px}
					, spaces::window::vector{_texture->width(), _texture->height()}
					};
				the_renderer().draw_box(outline, 1_px, colors::red(), colors::clear());
			}

			for (auto& kv : data.colored_hitboxes) {
				auto [hitbox, color] = kv.second;
				for (auto& vertex : hitbox.vertices) {
					vertex += offset_to_center;
				}
				the_renderer().draw_polygon(hitbox, 2.0_view_length, colors::black(), color);
			}
		}, solid_program());
	}
}
