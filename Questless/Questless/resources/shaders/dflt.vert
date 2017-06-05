#version 450

uniform vec2 window_size;

in vec2 position;
in vec2 texture_coords;
in vec4 color_factor;

out vec2 frag_texture_coords;
out vec4 frag_color_factor;

void main() {
	frag_texture_coords = texture_coords;
	frag_color_factor = color_factor;

	mat4 projection_matrix = mat4
		( vec4(2.0 / window_size.x, 0.0, 0.0, 0.0)
		, vec4(0.0, -2.0 / window_size.y, 0.0, 0.0)
		, vec4(0.0, 0.0, -1.0, 0.0)
		, vec4(-1.0, 1.0, 0.0, 1.0)
		);
	gl_Position = projection_matrix * vec4(position.x, position.y, 0, 1);
}
