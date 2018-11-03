//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "renderer.hpp"

#include "resources.hpp"

#include "units/math.hpp"

#include <gl/GL.h>
#include <gl/GLU.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <numeric>

using std::vector;
using std::runtime_error;

using namespace units;

//! @todo Can use ranges algorithms for the various vector<window_space::point> -> vector<ViewSpace::point> conversions, when available.

namespace sdl {
	namespace {
		bool has_positive_winding_order(view_space::polygon const& polygon) {
			// Winding order is positive iff "winding" is positive.
			meta::product_t<view, view> winding{0.0f};
			for (std::size_t i = 0; i < polygon.size(); ++i) {
				auto const p1 = polygon[i];
				auto const p2 = i == polygon.size() - 1 ? polygon.front() : polygon[i + 1];

				winding += (p1.x() - p2.x()) * (p2.y() + p1.y());
			}
			return winding >= meta::product_t<view, view>{0.0f};
		}

		bool is_convex(vector<view_space::point> const& vertices) {
			auto last_cross_product = math::square(0.0_view);
			for (std::size_t i = 0; i < vertices.size(); ++i) {
				auto const p0 = i == 0 ? vertices.back() : vertices[i - 1];
				auto const p1 = vertices[i];
				auto const p2 = i == vertices.size() - 1 ? vertices.front() : vertices[i + 1];

				auto cross_product = (p1.x() - p0.x()) * p2.y() - 2 * p1.y() - p0.y() * p2.x() - p1.x();
				if (last_cross_product == math::square(0.0_view)) {
					last_cross_product = cross_product;
				} else if (last_cross_product * cross_product < math::pow<4>(0.0_view)) {
					return false;
				}
			}
			return true;
		}

		vector<vector<view_space::point>> convex_components(vector<view_space::point> polygon) {
			//! @todo Implement.
			return {polygon};
		}
	}

	renderer::renderer(window& the_window, int width, int height) : _w{width}, _h{height}, _target{nullptr} {
		_renderer = SDL_CreateRenderer(the_window.sdl_ptr(), -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
		if (_renderer == nullptr) {
			throw runtime_error{"Failed to create renderer."};
		}

		if (SDL_RenderSetLogicalSize(_renderer, width, height)) {
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

	void renderer::draw_lines(vector<view_space::point> const& vertices, colors::color color) {
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
		vector<GLfloat> vertex_data;
		vertex_data.reserve(2 * vertices.size());
		for (auto const& vertex : vertices) {
			vertex_data.push_back(vertex.x().value);
			vertex_data.push_back(vertex.y().value);
		}
		glBufferData(GL_ARRAY_BUFFER, 2 * vertices.size() * sizeof(GLfloat), vertex_data.data(), GL_DYNAMIC_DRAW);
		{
			GLint const component_count = 2;
			GLsizei const stride = 2 * sizeof(GLfloat);
			void *const pointer_offset = nullptr;
			glVertexAttribPointer(position_attribute, component_count, GL_FLOAT, GL_FALSE, stride, pointer_offset);
		}

		{ // Set IBO.
			vector<GLuint> index_data(vertices.size());
			std::iota(index_data.begin(), index_data.end(), 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data.size() * sizeof(GLuint), index_data.data(), GL_DYNAMIC_DRAW);
		}

		// Set color uniform.
		GLuint color_uniform = solid_program().get_uniform_handle("color"); //! @todo Cache this, invalidating cache after window resize.
		glUniform4f(color_uniform, color.red().value, color.green().value, color.blue().value, color.alpha().value);

		// Render.
		glDrawElements(GL_LINE_STRIP, vertices.size(), GL_UNSIGNED_INT, nullptr);

		// Disable vertex attributes.
		glDisableVertexAttribArray(position_attribute);
	}
	
	void renderer::draw_lines(vector<window_space::point> const& vertices, colors::color color) {
		vector<view_space::point> view_space_vertices;
		for (auto const& vertex : vertices) {
			view_space_vertices.push_back(to_view_space(vertex));
		}
		draw_lines(view_space_vertices, color);
	}

	void renderer::draw_polygon(view_space::polygon const& polygon, colors::color color) {
		// Don't bother rendering transparent polygons.
		if (color.alpha() == color_component{0.0f}) { return; }

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
			vector<GLfloat> vertex_data;
			vertex_data.reserve(2 * polygon.size());
			for (auto const& vertex : polygon) {
				vertex_data.push_back(vertex.x().value);
				vertex_data.push_back(vertex.y().value);
			}
			glBufferData(GL_ARRAY_BUFFER, 2 * polygon.size() * sizeof(GLfloat), vertex_data.data(), GL_DYNAMIC_DRAW);
			{
				GLint const component_count = 2;
				GLsizei const stride = 2 * sizeof(GLfloat);
				void *const pointer_offset = nullptr;
				glVertexAttribPointer(position_attribute, component_count, GL_FLOAT, GL_FALSE, stride, pointer_offset);
			}
		}

		{ // Set IBO.
			vector<GLuint> index_data(polygon.size());
			std::iota(index_data.begin(), index_data.end(), 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data.size() * sizeof(GLuint), index_data.data(), GL_DYNAMIC_DRAW);
		}

		// Set color uniform.
		GLuint const color_uniform = solid_program().get_uniform_handle("color"); //! @todo Cache this, invalidating cache after window resize.
		glUniform4f(color_uniform, color.red().value, color.green().value, color.blue().value, color.alpha().value);

		// Render.
		glDrawElements(GL_TRIANGLE_FAN, polygon.size(), GL_UNSIGNED_INT, nullptr);

		// Disable vertex attributes.
		glDisableVertexAttribArray(position_attribute);
	}

	void renderer::draw_polygon(window_space::polygon const& polygon, colors::color color) {
		view_space::polygon view_space_polygon;
		for (auto const& vertex : polygon) {
			view_space_polygon.push_back(to_view_space(vertex));
		}
		draw_polygon(view_space_polygon, color);
	}
	
	void renderer::draw_polygon
		( view_space::polygon const& polygon
		, view_space::scalar border_width
		, colors::color border_color
		, colors::color fill_color
		)
	{
		// A polygon must have at least three vertices.
		if (polygon.size() < 3) { return; }

		// Construct fill vertices.
		view_space::polygon fill_vertices;
		bool const positive_winding_order = has_positive_winding_order(polygon);
		for (std::size_t i = 0; i < polygon.size(); ++i) {
			auto const p_minus_1 = i == 0 ? polygon.back() : polygon[i - 1];
			auto const p_plus_1 = i == polygon.size() - 1 ? polygon.front() : polygon[i + 1];

			// Edge direction depends on winding order.
			auto const p0 = positive_winding_order ? p_minus_1 : p_plus_1;
			auto const p1 = polygon[i];
			auto const p2 = positive_winding_order ? p_plus_1 : p_minus_1;

			// Parametric 2D line type.
			struct line { view_space::point p; view_space::vector m; };

			line l1{p0, p1 - p0};
			line l2{p1, p2 - p1};

			// If the segments are precisely parallel, ignore this vertex.
			if (l1.m.y() * l2.m.x() == l2.m.y() * l1.m.x()) { continue; }

			auto offset_start_point = [border_width](line& l) {
				// Find perpendicular slopes.
				view_space::vector m_prime{-l.m.y(), l.m.x()};
				// Normalize to border width.
				m_prime *= border_width / m_prime.length();
				// Offset first segment's starting point.
				l.p += m_prime;
			};
			offset_start_point(l1);
			offset_start_point(l2);

			// Find the intersection and add it to fill vertices. (Guaranteed to be unique since we ignore parallel segments.)
			auto l2_t_intersect = ((l2.p.y() - l1.p.y()) * l1.m.x() - (l2.p.x() - l1.p.x()) * l1.m.y()) / (l2.m.x() * l1.m.y() - l2.m.y() * l1.m.x());
			fill_vertices.push_back(l2.p + l2.m * l2_t_intersect);
		}

		// Draw fill color.
		draw_polygon(fill_vertices, fill_color);

		// Draw border color.
		vector<view_space::point> border_vertices;
		for (std::size_t i = 0; i < polygon.size(); ++i) {
			border_vertices.push_back(polygon[i]);
			border_vertices.push_back(fill_vertices[i]);
		}
		// Close the loop by repeating the first two vertices.
		border_vertices.push_back(polygon.front());
		border_vertices.push_back(fill_vertices.front());
		draw_triangle_strip(border_vertices, border_color);
	}
	
	void renderer::draw_polygon
		( window_space::polygon const& polygon
		, window_space::scalar border_width
		, colors::color border_color
		, colors::color fill_color
		)
	{
		view_space::polygon view_space_polygon;
		for (auto const& vertex : polygon) {
			view_space_polygon.push_back(to_view_space(vertex));
		}
		draw_polygon(view_space_polygon, to_view_space(border_width), border_color, fill_color);
	}

	void renderer::draw_box(view_space::box const& box, colors::color color)
	{
		draw_polygon(view_space::polygon{top_left(box), top_right(box), bottom_right(box), bottom_left(box)}, color);
	}

	void renderer::draw_box(window_space::box const& box, colors::color color) {
		draw_box(to_view_space(box), color);
	}

	void renderer::draw_box
		( window_space::box const& box
		, window_space::scalar border_width
		, colors::color border_color
		, colors::color fill_color
		) {
		if (units::width(box) > 0_px && units::height(box) > 0_px) {
			draw_box(to_view_space(box), to_view_space(border_width), border_color, fill_color);
		}
	}
	
	void renderer::draw_box
		( view_space::box const& box
		, view_space::scalar border_width
		, colors::color border_color
		, colors::color fill_color
		) {
		if (units::width(box) > 0.0_view && units::height(box) > 0.0_view) {
			draw_polygon(view_space::polygon{top_left(box), top_right(box), bottom_right(box), bottom_left(box)}, border_width, border_color, fill_color);
		}
	}

	void renderer::draw_disc
		( view_space::sphere const& boundary
		, view_space::scalar border_width
		, colors::color border_color
		, colors::color fill_color
		, float segments_per_radius
		)
	{
		view_space::polygon polygon;
		int const segments_count = lround(boundary.radius.value * segments_per_radius);
		for (int i = 0; i < segments_count; ++i) {
			view_space::radians const angle{static_cast<float>(constants::tau * i / segments_count)};
			view_space::vector const offset{angle, boundary.radius};
			polygon.push_back(boundary.center + offset);
		}
		draw_polygon(polygon, border_width, border_color, fill_color);
	}

	void renderer::draw_disc
		( window_space::sphere const& boundary
		, window_space::scalar border_width
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
			, static_cast<uint8_t>(255 * color.red().value)
			, static_cast<uint8_t>(255 * color.green().value)
			, static_cast<uint8_t>(255 * color.blue().value)
			, static_cast<uint8_t>(255 * color.alpha().value)
			);
		if (error) {
			std::string message = std::string{"Failed to set renderer draw color. SDL error: "} + SDL_GetError();
			throw runtime_error{message};
		}
	}

	void renderer::draw_triangle_strip(vector<view_space::point> const& vertices, colors::color color) {
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
			vector<GLfloat> vertex_data;
			vertex_data.reserve(2 * vertices.size());
			for (auto const& vertex : vertices) {
				vertex_data.push_back(vertex.x().value);
				vertex_data.push_back(vertex.y().value);
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
			vector<GLuint> index_data(vertices.size());
			std::iota(index_data.begin(), index_data.end(), 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data.size() * sizeof(GLuint), index_data.data(), GL_DYNAMIC_DRAW);
		}

		// Set color uniform.
		GLuint const color_uniform = solid_program().get_uniform_handle("color"); //! @todo Cache this, invalidating cache after window resize.
		glUniform4f(color_uniform, color.red().value, color.green().value, color.blue().value, color.alpha().value);

		// Render.
		glDrawElements(GL_TRIANGLE_STRIP, vertices.size(), GL_UNSIGNED_INT, nullptr);

		// Disable vertex attributes.
		glDisableVertexAttribArray(position_attribute);
	}
}
