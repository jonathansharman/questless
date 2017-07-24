#version 450

uniform sampler2D sampler;

in vec2 frag_texture_coords;
in vec4 frag_color_factor;

out vec4 fragment;

void main() {
	fragment = texture(sampler, frag_texture_coords) * frag_color_factor.rgba;
}
