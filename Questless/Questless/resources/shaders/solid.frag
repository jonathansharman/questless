#version 450

uniform vec4 color;

out vec4 fragment;

void main() {
	fragment = color.rgba;
}
