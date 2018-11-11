//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "renderer.hpp"

#include "resources.hpp"

#include <gl/GL.h>
#include <gl/GLU.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <numeric>

using std::runtime_error;

using namespace vecx;

namespace sdl {
	using namespace spaces;
	using namespace spaces::colors::literals;
	using namespace spaces::view::literals;
	
	namespace {
		bool has_positive_winding_order(view::polygon const& polygon) {
			// Winding order is positive iff "winding" is positive.
			auto winding = 0.0_view_length * 0.0_view_length;
			for (std::size_t i = 0; i < polygon.vertices.size(); ++i) {
				auto const p1 = polygon.vertices[i];
				auto const p2 = i == polygon.vertices.size() - 1 ? polygon.vertices.front() : polygon.vertices[i + 1];

				winding += (x(p1) - x(p2)) * (y(p2) + y(p1));
			}
			return winding >= 0.0_view_length * 0.0_view_length;
		}

		bool is_convex(std::vector<view::point> const& vertices) {
			auto const v_squared = 0.0_view_length * 0.0_view_length;
			auto const v_fourth = v_squared * v_squared;
			auto last_cross_product = v_squared;
			for (std::size_t i = 0; i < vertices.size(); ++i) {
				auto const p0 = i == 0 ? vertices.back() : vertices[i - 1];
				auto const p1 = vertices[i];
				auto const p2 = i == vertices.size() - 1 ? vertices.front() : vertices[i + 1];

				auto cross_product = (x(p1) - x(p0)) * y(p2) - 2 * y(p1) - y(p0) * x(p2) - x(p1);
				if (last_cross_product == 0.0_view_length * 0.0_view_length){
					last_cross_product = cross_product;
				} else if (last_cross_product * cross_product < v_fourth) {
					return false;
				}
			}
			return true;
		}

		std::vector<std::vector<view::point>> convex_components(std::vector<view::point> polygon) {
			//! @todo Implement.
			return {polygon};
		}
	}

	renderer::renderer(sdl::window& the_window, spaces::window::px width, spaces::window::px height) : _w{width}, _h{height}, _target{nullptr} {
		_renderer = SDL_CreateRenderer(the_window.sdl_ptr(), -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
		if (_renderer == nullptr) {
			throw runtime_error{"Failed to create renderer."};
		}

		if (SDL_RenderSetLogicalSize(_renderer, width.value, height.value)) {
			throw runtime_error{"Failed to set renderer's logical size."};
		}

		if (SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND)) {
			throw runtime_error{"Failed to set renderer blend mode."};
		}

		if (SDL_GetRendererInfo(_renderer, &_info)) {
			throw runtime_error{"Failed to get renderer info."};
		}

		// Create VBO.
		glGenBuffers(1, &_vbo);

		// Create IBO.
		glGenBuffers(1, &_ibo);
	}

	renderer::~renderer() {
		SDL_DestroyRenderer(_renderer);

		// Delete buffer objects.
		glDeleteBuffers(1, &_vbo);
		glDeleteBuffers(1, &_ibo);
	}

	SDL_Texture* renderer::target() const {
		return _target;
	}

	void renderer::target(SDL_Texture* target) {
		if (SDL_SetRenderTarget(_renderer, target)) {
			throw runtime_error("Failed to set render target.");
		} else {
			_target = target;
		}
	}

	void renderer::clear(colors::color color) {
		set_draw_color(color);
		SDL_RenderClear(_renderer);
	}

	void renderer::draw_lines(std::vector<view::point> const& vertices, colors::color color) {
		// Bind program.
		solid_program().use();

		// Enable vertex attributes.
		GLuint const position_attribute = solid_program().get_attribute_handle("position"); //! @todo Cache this, invalidating cache after window resize.
		glEnableVertexAttribArray(position_attribute);

		{ // Set model matrix to identity.
			glm::mat4 model_matrix;
			GLuint const model_matrix_uniform = solid_program().get_uniform_handle("model_matrix"); //! @todo Cache this, invalidating cache after window resize.
			glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, glm::value_ptr(model_matrix));
		}

		// Set vertex data.
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		std::vector<GLfloat> vertex_data;
		vertex_data.reserve(2 * vertices.size());
		for (auto const& vertex : vertices) {
			vertex_data.push_back(x(vertex).value);
			vertex_data.push_back(y(vertex).value);
		}
		glBufferData(GL_ARRAY_BUFFER, 2 * vertices.size() * sizeof(GLfloat), vertex_data.data(), GL_DYNAMIC_DRAW);
		{
			GLint const component_count = 2;
			GLsizei const stride = 2 * sizeof(GLfloat);
			void *const pointer_offset = nullptr;
			glVertexAttribPointer(position_attribute, component_count, GL_FLOAT, GL_FALSE, stride, pointer_offset);
		}

		{ // Set IBO.
			std::vector<GLuint> index_data(vertices.size());
			std::iota(index_data.begin(), index_data.end(), 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data.size() * sizeof(GLuint), index_data.data(), GL_DYNAMIC_DRAW);
		}

		// Set color uniform.
		GLuint color_uniform = solid_program().get_uniform_handle("color"); //! @todo Cache this, invalidating cache after window resize.
		glUniform4f(color_uniform, r(color).value, g(color).value, b(color).value, a(color).value);

		// Render.
		glDrawElements(GL_LINE_STRIP, vertices.size(), GL_UNSIGNED_INT, nullptr);

		// Disable vertex attributes.
		glDisableVertexAttribArray(position_attribute);
	}
	
	void renderer::draw_lines(std::vector<spaces::window::point> const& vertices, colors::color color) {
		std::vector<view::point> view_space_vertices;
		for (auto const& vertex : vertices) {
			view_space_vertices.push_back(to_view_space(vertex));
		}
		draw_lines(view_space_vertices, color);
	}

	void renderer::draw_polygon(view::polygon const& polygon, colors::color color) {
		// Don't bother rendering transparent polygons.
		if (a(color) == 0.0_c) { return; }

		//! @todo Reenable after convex_components() is implemented.
		// Break solid non-convex polygons into convex subpolygons.
		//if (fill == Fill::solid && !is_convex(polygon)) {
		//	for (auto const& polygon : convex_components(polygon)) {
		//		draw_polygon(polygon, color, Fill::solid);
		//	}
		//	return;
		//}

		// Bind program.
		solid_program().use();

		// Enable vertex attributes.
		GLuint const position_attribute = solid_program().get_attribute_handle("position"); //! @todo Cache this, invalidating cache after window resize.
		glEnableVertexAttribArray(position_attribute);

		{ // Set model matrix to identity.
			glm::mat4 model_matrix;
			GLuint const model_matrix_uniform = solid_program().get_uniform_handle("model_matrix"); //! @todo Cache this, invalidating cache after window resize.
			glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, glm::value_ptr(model_matrix));
		}

		{ // Set vertex data.
			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			std::vector<GLfloat> vertex_data;
			vertex_data.reserve(2 * polygon.vertices.size());
			for (auto const& vertex : polygon.vertices) {
				vertex_data.push_back(x(vertex).value);
				vertex_data.push_back(y(vertex).value);
			}
			glBufferData(GL_ARRAY_BUFFER, 2 * polygon.vertices.size() * sizeof(GLfloat), vertex_data.data(), GL_DYNAMIC_DRAW);
			{
				GLint const component_count = 2;
				GLsizei const stride = 2 * sizeof(GLfloat);
				void *const pointer_offset = nullptr;
				glVertexAttribPointer(position_attribute, component_count, GL_FLOAT, GL_FALSE, stride, pointer_offset);
			}
		}

		{ // Set IBO.
			std::vector<GLuint> index_data(polygon.vertices.size());
			std::iota(index_data.begin(), index_data.end(), 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data.size() * sizeof(GLuint), index_data.data(), GL_DYNAMIC_DRAW);
		}

		// Set color uniform.
		GLuint const color_uniform = solid_program().get_uniform_handle("color"); //! @todo Cache this, invalidating cache after window resize.
		glUniform4f(color_uniform, r(color).value, g(color).value, b(color).value, a(color).value);

		// Render.
		glDrawElements(GL_TRIANGLE_FAN, polygon.vertices.size(), GL_UNSIGNED_INT, nullptr);

		// Disable vertex attributes.
		glDisableVertexAttribArray(position_attribute);
	}

	void renderer::draw_polygon(spaces::window::polygon const& polygon, colors::color color) {
		draw_polygon(to_view_space(polygon), color);
	}
	
	void renderer::draw_polygon
		( view::polygon const& polygon
		, view::length border_width
		, colors::color border_color
		, colors::color fill_color
		)
	{
		// A polygon must have at least three vertices.
		if (polygon.vertices.size() < 3) { return; }

		// Construct fill vertices.
		view::polygon fill_polygon;
		bool const positive_winding_order = has_positive_winding_order(polygon);
		for (std::size_t i = 0; i < polygon.vertices.size(); ++i) {
			auto const p_minus_1 = i == 0 ? polygon.vertices.back() : polygon.vertices[i - 1];
			auto const p_plus_1 = i == polygon.vertices.size() - 1 ? polygon.vertices.front() : polygon.vertices[i + 1];

			// Edge direction depends on winding order.
			auto const p0 = positive_winding_order ? p_minus_1 : p_plus_1;
			auto const p1 = polygon.vertices[i];
			auto const p2 = positive_winding_order ? p_plus_1 : p_minus_1;

			// Parametric 2D line type.
			struct line { view::point p; view::vector m; };

			line l1{p0, p1 - p0};
			line l2{p1, p2 - p1};

			// If the segments are precisely parallel, ignore this vertex.
			if (y(l1.m) * x(l2.m) == y(l2.m) * x(l1.m)) { continue; }

			auto offset_start_point = [border_width](line& l) {
				// Find perpendicular slopes.
				view::vector m_prime{-y(l.m), x(l.m)};
				// Normalize to border width.
				m_prime *= border_width / m_prime.length();
				// Offset first segment's starting point.
				l.p += m_prime;
			};
			offset_start_point(l1);
			offset_start_point(l2);

			// Find the intersection and add it to fill vertices. (Guaranteed to be unique since we ignore parallel segments.)
			auto l2_t_intersect = ((y(l2.p) - y(l1.p)) * x(l1.m) - (x(l2.p) - x(l1.p)) * y(l1.m)) / (x(l2.m) * y(l1.m) - y(l2.m) * x(l1.m));
			fill_polygon.vertices.push_back(l2.p + l2.m * l2_t_intersect);
		}

		// Draw fill color.
		draw_polygon(fill_polygon, fill_color);

		// Draw border color.
		std::vector<view::point> border_vertices;
		for (std::size_t i = 0; i < polygon.vertices.size(); ++i) {
			border_vertices.push_back(polygon.vertices[i]);
			border_vertices.push_back(fill_polygon.vertices[i]);
		}
		// Close the loop by repeating the first two vertices.
		border_vertices.push_back(polygon.vertices.front());
		border_vertices.push_back(fill_polygon.vertices.front());
		draw_triangle_strip(border_vertices, border_color);
	}
	
	void renderer::draw_polygon
		( spaces::window::polygon const& polygon
		, spaces::window::px border_width
		, colors::color border_color
		, colors::color fill_color
		)
	{
		draw_polygon(to_view_space(polygon), to_view_space(border_width), border_color, fill_color);
	}

	void renderer::draw_box(view::box const& box, colors::color color)
	{
		draw_polygon(view::polygon{top_left(box), top_right(box), bottom_right(box), bottom_left(box)}, color);
	}

	void renderer::draw_box(spaces::window::box const& box, colors::color color) {
		draw_box(to_view_space(box), color);
	}

	void renderer::draw_box
		( spaces::window::box const& box
		, spaces::window::px border_width
		, colors::color border_color
		, colors::color fill_color
		)
	{
		draw_box(to_view_space(box), to_view_space(border_width), border_color, fill_color);
	}
	
	void renderer::draw_box
		( view::box const& box
		, view::length border_width
		, colors::color border_color
		, colors::color fill_color
		)
	{
		if (spaces::view::width(box) > 0.0_view_length && spaces::view::height(box) > 0.0_view_length) {
			draw_polygon(view::polygon{top_left(box), top_right(box), bottom_right(box), bottom_left(box)}, border_width, border_color, fill_color);
		}
	}

	void renderer::draw_disc
		( view::circle const& boundary
		, view::length border_width
		, colors::color border_color
		, colors::color fill_color
		, float segments_per_radius
		)
	{
		view::polygon polygon;
		int const segments_count = lround(boundary.radius.value * segments_per_radius);
		for (int i = 0; i < segments_count; ++i) {
			radians const angle{static_cast<float>(constants::tau * i / segments_count)};
			auto const offset = vecx::make_polar_vector(boundary.radius, angle);
			polygon.vertices.push_back(boundary.center + offset);
		}
		draw_polygon(polygon, border_width, border_color, fill_color);
	}

	void renderer::draw_disc
		( spaces::window::circle const& boundary
		, spaces::window::px border_width
		, colors::color border_color
		, colors::color fill_color
		, float segments_per_radius
		)
	{
		draw_disc(to_view_space(boundary), to_view_space(border_width), border_color, fill_color, segments_per_radius);
	}

	void renderer::set_draw_color(colors::color color) {
		auto error = SDL_SetRenderDrawColor
			( _renderer
			, static_cast<uint8_t>(255 * r(color).value)
			, static_cast<uint8_t>(255 * g(color).value)
			, static_cast<uint8_t>(255 * b(color).value)
			, static_cast<uint8_t>(255 * a(color).value)
			);
		if (error) {
			std::string message = std::string{"Failed to set renderer draw color. SDL error: "} + SDL_GetError();
			throw runtime_error{message};
		}
	}

	void renderer::draw_triangle_strip(std::vector<view::point> const& vertices, colors::color color) {
		// Bind program.
		solid_program().use();

		// Enable vertex attributes.
		GLuint const position_attribute = solid_program().get_attribute_handle("position"); //! @todo Cache this, invalidating cache after window resize.
		glEnableVertexAttribArray(position_attribute);

		{ // Set model matrix to identity.
			glm::mat4 model_matrix;
			GLuint const model_matrix_uniform = solid_program().get_uniform_handle("model_matrix"); //! @todo Cache this, invalidating cache after window resize.
			glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, glm::value_ptr(model_matrix));
		}

		{ // Set vertex data.
			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			std::vector<GLfloat> vertex_data;
			vertex_data.reserve(2 * vertices.size());
			for (auto const& vertex : vertices) {
				vertex_data.push_back(x(vertex).value);
				vertex_data.push_back(y(vertex).value);
			}
			glBufferData(GL_ARRAY_BUFFER, 2 * vertices.size() * sizeof(GLfloat), vertex_data.data(), GL_DYNAMIC_DRAW);
			{
				GLint const component_count = 2;
				GLsizei const stride = 2 * sizeof(GLfloat);
				void *const pointer_offset = nullptr;
				glVertexAttribPointer(position_attribute, component_count, GL_FLOAT, GL_FALSE, stride, pointer_offset);
			}
		}

		{ // Set IBO.
			std::vector<GLuint> index_data(vertices.size());
			std::iota(index_data.begin(), index_data.end(), 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data.size() * sizeof(GLuint), index_data.data(), GL_DYNAMIC_DRAW);
		}

		// Set color uniform.
		GLuint const color_uniform = solid_program().get_uniform_handle("color"); //! @todo Cache this, invalidating cache after window resize.
		glUniform4f(color_uniform, r(color).value, g(color).value, b(color).value, a(color).value);

		// Render.
		glDrawElements(GL_TRIANGLE_STRIP, vertices.size(), GL_UNSIGNED_INT, nullptr);

		// Disable vertex attributes.
		glDisableVertexAttribArray(position_attribute);
	}
}
