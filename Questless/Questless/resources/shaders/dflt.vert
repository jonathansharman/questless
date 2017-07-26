#version 450

uniform vec2 viewport_size;
uniform bool flip_y;

in vec2 position;
in vec2 texture_coords;

out vec2 frag_texture_coords;

void main() {
	frag_texture_coords = texture_coords;
	
	float t = flip_y ? viewport_size.y : 0.0;
	float b = flip_y ? 0.0 : viewport_size.y;
	float l = 0.0;
	float r = viewport_size.x;
	float f = -1.0;
	float n = 1.0;
	mat4 projection_matrix = mat4
		( vec4(2.0 / (r - l), 0.0, 0.0, 0.0)
		, vec4(0.0, 2.0 / (t - b), 0.0, 0.0)
		, vec4(0.0, 0.0, -2.0 / (f - n), 0.0)
		, vec4(-(r + l) / (r - l), -(t + b) / (t - b), -(f + n) / (f - n), 1.0)
		);
	gl_Position = projection_matrix * vec4(position.x, position.y, 0, 1);
}
