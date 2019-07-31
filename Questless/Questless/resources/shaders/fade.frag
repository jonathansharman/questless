#version 450

uniform float intensity;

out vec4 fragment;

void main() {
	fragment = vec4(color.rgb * intensity, color.a);
}
