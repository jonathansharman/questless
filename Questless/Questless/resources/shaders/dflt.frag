#version 450

uniform sampler2D sampler;
uniform vec4 color_factor;

in vec2 frag_texture_coords;

out vec4 fragment;

void main() {
	fragment = texture(sampler, frag_texture_coords) * color_factor.rgba;
}
