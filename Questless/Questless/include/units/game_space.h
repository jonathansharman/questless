//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#pragma once

#include "rate.h"
#include "space.h"

namespace units
{
	namespace detail
	{
		class game_space_buffer : public buffer_base<double, 2>
		{
		public:
			double& x() & { return _elements[0]; }
			constexpr double const& x() const& { return _elements[0]; }

			double& y() & { return _elements[1]; }
			constexpr double const& y() const& { return _elements[1]; }
		protected:
			using buffer_base::buffer_base;
			using buffer_base::operator =;
		};
	}

	struct game_space : space<struct game_space_tag, double, 2, detail::game_space_buffer>
	{
		using h_align = axis<0>::align;
		using v_align = axis<1>::align;

		static constexpr auto align_left = h_align::near;
		static constexpr auto align_center = h_align::mid;
		static constexpr auto align_right = h_align::far;

		static constexpr auto align_top = v_align::near;
		static constexpr auto align_middle = v_align::mid;
		static constexpr auto align_bottom = v_align::far;

		// Timing and Rates

		using seconds = std::chrono::duration<double>;

		using radians_per_sec = rate<radians, scalar>;

		using speed = rate<scalar, scalar>;
		using velocity = rate<vector, scalar>;
		using acceleration = rate<velocity, scalar>;

		//! @todo: How to deal with speed and scale_velocity being the same type?
		using scale_velocity = rate<scalar, scalar>;
	};

	inline double& width(game_space::box& box) { return box.size.x(); }
	inline double width(game_space::box const& box) { return box.size.x(); }

	inline double& height(game_space::box& box) { return box.size.y(); }
	inline double height(game_space::box const& box) { return box.size.y(); }

	inline double& left(game_space::box& box) { return box.position.x(); }
	inline double left(game_space::box const& box) { return box.position.x(); }

	inline double& top(game_space::box& box) { return box.position.y(); }
	inline double top(game_space::box const& box) { return box.position.y(); }

	inline double right(game_space::box const& box) { return box.position.x() + box.size.x(); }

	inline double bottom(game_space::box const& box) { return box.position.y() + box.size.y(); }

	inline game_space::point top_left(game_space::box const& box) { return box.position; }
	inline game_space::point top_right(game_space::box const& box) { return game_space::point{box.position.x() + box.size.x(), box.position.y()}; }
	inline game_space::point bottom_left(game_space::box const& box) { return game_space::point{box.position.x(), box.position.y() + box.size.y()}; }
	inline game_space::point bottom_right(game_space::box const& box) { return box.position + box.size; }

	inline game_space::point center(game_space::box const& box) { return box.position + box.size / 2.0; }
}

// Bring chrono_literals into the global namespace, for convenience when dealing with game time.
using namespace std::literals::chrono_literals;
