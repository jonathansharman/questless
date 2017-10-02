//! @file
//! @author Jonathan Sharman
//! @copyright See <a href='../../LICENSE.txt'>LICENSE.txt</a>.

#include "sdl/Renderer.h"

#include <numeric>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "sdl/resources.h"

using std::vector;
using std::runtime_error;

using namespace units;

namespace sdl
{
	Renderer::Renderer(Window& window, int width, int height) : _w{width}, _h{height}, _target{nullptr}
	{
		_renderer = SDL_CreateRenderer(window.sdl_ptr(), -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
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

		{ // Create IBO.
			GLuint index_data[] { 0, 1, 2, 3 };
			glGenBuffers(1, &_quad_ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quad_ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), index_data, GL_DYNAMIC_DRAW);
		}
	}

	Renderer::~Renderer()
	{
		SDL_DestroyRenderer(_renderer);
	}

	SDL_Texture* Renderer::target() const
	{
		return _target;
	}

	void Renderer::target(SDL_Texture* target)
	{
		if (SDL_SetRenderTarget(_renderer, target)) {
			throw runtime_error("Failed to set render target.");
		} else {
			_target = target;
		}
	}

	void Renderer::clear(colors::Color color)
	{
		set_draw_color(color);
		SDL_RenderClear(_renderer);
	}

	void Renderer::draw_lines(vector<ScreenSpace::Point> vertices, colors::Color color)
	{
		// Bind program.
		solid_program().use();

		// Enable vertex attributes.
		static GLuint position_attribute = solid_program().get_attribute_handle("position");
		glEnableVertexAttribArray(position_attribute);

		{ // Set model matrix to identity.
			glm::mat4 model_matrix;
			static GLuint model_matrix_uniform = solid_program().get_uniform_handle("model_matrix");
			glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, glm::value_ptr(model_matrix));
		}

		// Set vertex data.
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		vector<GLfloat> vertex_data;
		vertex_data.reserve(2 * vertices.size());
		for (auto const& vertex : vertices) {
			vertex_data.push_back(static_cast<GLfloat>(vertex.x()));
			vertex_data.push_back(static_cast<GLfloat>(vertex.y()));
		}
		glBufferData(GL_ARRAY_BUFFER, 2 * vertices.size() * sizeof(GLfloat), vertex_data.data(), GL_DYNAMIC_DRAW);
		{
			GLint const component_count = 2;
			GLsizei const stride = 2 * sizeof(GLfloat);
			void *const pointer_offset = nullptr;
			glVertexAttribPointer(position_attribute, component_count, GL_FLOAT, GL_FALSE, stride, pointer_offset);
		}

		GLuint lines_ibo;
		{ // Create IBO.
			vector<GLuint> index_data(vertices.size());
			std::iota(index_data.begin(), index_data.end(), 0);
			glGenBuffers(1, &lines_ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lines_ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data.size() * sizeof(GLuint), index_data.data(), GL_DYNAMIC_DRAW);
		}

		// Set color uniform.
		static GLuint color_uniform = solid_program().get_uniform_handle("color");
		glUniform4f(color_uniform, color.red(), color.green(), color.blue(), color.alpha());

		// Render.
		glDrawElements(GL_LINE_STRIP, vertices.size(), GL_UNSIGNED_INT, nullptr);

		// Disable vertex attributes.
		glDisableVertexAttribArray(position_attribute);

		// Delete IBO.
		glDeleteBuffers(1, &lines_ibo);
	}

	void Renderer::draw_polygon(vector<ViewSpace::Point> vertices, colors::Color color, Fill fill)
	{
		// Bind program.
		solid_program().use();

		// Enable vertex attributes.
		static GLuint position_attribute = solid_program().get_attribute_handle("position");
		glEnableVertexAttribArray(position_attribute);

		{ // Set model matrix to identity.
			glm::mat4 model_matrix;
			static GLuint model_matrix_uniform = solid_program().get_uniform_handle("model_matrix");
			glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, glm::value_ptr(model_matrix));
		}

		{ // Set vertex data.
			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			vector<GLfloat> vertex_data;
			vertex_data.reserve(2 * vertices.size());
			for (auto const& vertex : vertices) {
				vertex_data.push_back(vertex.x());
				vertex_data.push_back(vertex.y());
			}
			glBufferData(GL_ARRAY_BUFFER, 2 * vertices.size() * sizeof(GLfloat), vertex_data.data(), GL_DYNAMIC_DRAW);
			{
				GLint const component_count = 2;
				GLsizei const stride = 2 * sizeof(GLfloat);
				void *const pointer_offset = nullptr;
				glVertexAttribPointer(position_attribute, component_count, GL_FLOAT, GL_FALSE, stride, pointer_offset);
			}
		}

		GLuint polygon_ibo;
		{ // Create IBO.
			vector<GLuint> index_data(vertices.size());
			std::iota(index_data.begin(), index_data.end(), 0);
			glGenBuffers(1, &polygon_ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, polygon_ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data.size() * sizeof(GLuint), index_data.data(), GL_DYNAMIC_DRAW);
		}

		// Set color uniform.
		static GLuint color_uniform = solid_program().get_uniform_handle("color");
		glUniform4f(color_uniform, color.red(), color.green(), color.blue(), color.alpha());

		// Render.
		switch (fill) {
			case Fill::solid:
				glDrawElements(GL_POLYGON, vertices.size(), GL_UNSIGNED_INT, nullptr);
				break;
			case Fill::outline:
				glDrawElements(GL_LINE_LOOP, vertices.size(), GL_UNSIGNED_INT, nullptr);
				break;
		}

		// Disable vertex attributes.
		glDisableVertexAttribArray(position_attribute);

		// Delete IBO.
		glDeleteBuffers(1, &polygon_ibo);
	}
	
	void Renderer::draw_polygon
		( vector<ViewSpace::Point> vertices
		, ViewSpace::scalar_t border_width
		, colors::Color border_color
		, colors::Color fill_color
		)
	{
		// Draw border.
		draw_polygon(vertices, border_color, Fill::solid);

		//! @todo Compute winding order. (Can use interior angle sum formula to check.)

		vector<ViewSpace::Point> fill_vertices;
		for (std::size_t i = 0; i < vertices.size(); ++i) {
			auto p0 = i == 0 ? vertices.back() : vertices[i - 1];
			auto p1 = vertices[i];
			auto p2 = i == vertices.size() - 1 ? vertices.front() : vertices[i + 1];

			// Parametric 2D line type.
			struct Line { float x0, y0, mx, my; };

			Line l1{p0.x(), p0.y(), p1.x() - p0.x(), p1.y() - p0.y()};
			Line l2{p1.x(), p1.y(), p2.x() - p1.x(), p2.y() - p1.y()};

			// If the segments are precisely parallel, we can ignore this vertex.
			if (l1.my * l2.mx == l2.my * l1.mx) { continue; }

			// Find slopes perpendicular to first line.
			auto l1_mx_prime = -l1.my;
			auto l1_my_prime = l1.mx;
			// Normalize to border width.
			auto length1 = sqrt(l1_mx_prime * l1_mx_prime + l1_my_prime * l1_my_prime);
			l1_mx_prime = l1_mx_prime * border_width / length1;
			l1_my_prime = l1_my_prime * border_width / length1;
			// Offset first segment's starting point.
			l1.x0 += l1_mx_prime;
			l1.y0 += l1_my_prime;

			// L2 perpendicular slopes
			auto l2_mx_prime = -l2.my;
			auto l2_my_prime = l2.mx;
			// Normalize.
			auto length2 = sqrt(l2_mx_prime * l2_mx_prime + l2_my_prime * l2_my_prime);
			l2_mx_prime = l2_mx_prime * border_width / length2;
			l2_my_prime = l2_my_prime * border_width / length2;
			// Offset second segment's starting point.
			l2.x0 += l2_mx_prime;
			l2.y0 += l2_my_prime;

			// Find the intersection and add it to fill vertices. (Guaranteed to be unique since we 
			auto l2_t_intersect = ((l2.y0 - l1.y0) * l1.mx - (l2.x0 - l1.x0) * l1.my) / (l2.mx * l1.my - l2.my * l1.mx);
			fill_vertices.push_back(ViewSpace::Point{l2.x0 + l2.mx * l2_t_intersect, l2.y0 + l2.my * l2_t_intersect});
		}

		// Draw fill color.
		draw_polygon(fill_vertices, fill_color, Fill::solid);
	}

	void Renderer::draw_box(ScreenSpace::Box const& box, colors::Color color, Fill fill)
	{
		// Bind program.
		solid_program().use();

		// Enable vertex attributes.
		static GLuint position_attribute = solid_program().get_attribute_handle("position");
		glEnableVertexAttribArray(position_attribute);

		{ // Set model matrix to identity.
			glm::mat4 model_matrix;
			static GLuint model_matrix_uniform = solid_program().get_uniform_handle("model_matrix");
			glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, glm::value_ptr(model_matrix));
		}

		{ // Set vertex data.
			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			const float offset = fill == Fill::solid ? 0.0f : 0.375f;
			GLfloat vertex_data[] =
				{ left(box) + offset, top(box) + offset
				, right(box) - offset, top(box) + offset
				, right(box) - offset, bottom(box) - offset
				, left(box) + offset, bottom(box) - offset
				};
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_DYNAMIC_DRAW);
			{
				GLint const component_count = 2;
				GLsizei const stride = 2 * sizeof(GLfloat);
				void *const pointer_offset = nullptr;
				glVertexAttribPointer(position_attribute, component_count, GL_FLOAT, GL_FALSE, stride, pointer_offset);
			}
		}

		// Set index data.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quad_ibo);

		// Set color uniform.
		static GLuint color_uniform = solid_program().get_uniform_handle("color");
		glUniform4f(color_uniform, color.red(), color.green(), color.blue(), color.alpha());

		// Render.
		switch (fill) {
			case Fill::solid:
				glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, nullptr);
				break;
			case Fill::outline:
				glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, nullptr);
				break;
		}

		// Disable vertex attributes.
		glDisableVertexAttribArray(position_attribute);
	}

	void Renderer::draw_box(ScreenSpace::Box const& box, colors::Color border_color, colors::Color fill_color)
	{
		// Bind program.
		solid_program().use();

		// Enable vertex attributes.
		static GLuint position_attribute = solid_program().get_attribute_handle("position");
		glEnableVertexAttribArray(position_attribute);

		{ // Set model matrix to identity.
			glm::mat4 model_matrix;
			static GLuint model_matrix_uniform = solid_program().get_uniform_handle("model_matrix");
			glUniformMatrix4fv(model_matrix_uniform, 1, GL_FALSE, glm::value_ptr(model_matrix));
		}

		// Set index data.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quad_ibo);

		{ // Set fill box vertex data.
			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			GLfloat vertex_data[] =
				{ static_cast<float>(left(box)), static_cast<float>(top(box))
				, static_cast<float>(right(box)), static_cast<float>(top(box))
				, static_cast<float>(right(box)), static_cast<float>(bottom(box))
				, static_cast<float>(left(box)), static_cast<float>(bottom(box))
				};
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_DYNAMIC_DRAW);
			{
				GLint const component_count = 2;
				GLsizei const stride = 2 * sizeof(GLfloat);
				void *const pointer_offset = nullptr;
				glVertexAttribPointer(position_attribute, component_count, GL_FLOAT, GL_FALSE, stride, pointer_offset);
			}
		}

		// Set color uniform to fill color.
		static GLuint color_uniform = solid_program().get_uniform_handle("color");
		glUniform4f(color_uniform, fill_color.red(), fill_color.green(), fill_color.blue(), fill_color.alpha());

		// Render fill color.
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, nullptr);

		{ // Set border box vertex data.
			glBindBuffer(GL_ARRAY_BUFFER, _vbo);
			constexpr float offset = 0.375f;
			GLfloat vertex_data[] =
				{ left(box) + offset, top(box) + offset
				, right(box) - offset, top(box) + offset
				, right(box) - offset, bottom(box) - offset
				, left(box) + offset, bottom(box) - offset
				};
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_DYNAMIC_DRAW);
			{
				GLint const component_count = 2;
				GLsizei const stride = 2 * sizeof(GLfloat);
				void *const pointer_offset = nullptr;
				glVertexAttribPointer(position_attribute, component_count, GL_FLOAT, GL_FALSE, stride, pointer_offset);
			}
		}

		// Set color uniform to border color.
		glUniform4f(color_uniform, border_color.red(), border_color.green(), border_color.blue(), border_color.alpha());

		// Render border color.
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, nullptr);

		// Disable vertex attributes.
		glDisableVertexAttribArray(position_attribute);
	}

	void Renderer::set_draw_color(colors::Color color)
	{
		auto error = SDL_SetRenderDrawColor
			( _renderer
			, static_cast<uint8_t>(255 * color.red())
			, static_cast<uint8_t>(255 * color.green())
			, static_cast<uint8_t>(255 * color.blue())
			, static_cast<uint8_t>(255 * color.alpha())
			);
		if (error) {
			std::string message = std::string{"Failed to set renderer draw color. SDL error: "} + SDL_GetError();
			throw runtime_error{message};
		}
	}
}
