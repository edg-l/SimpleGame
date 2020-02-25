#version 330 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec4 color;

out vec4 tileColor;

uniform mat4 projection;
//uniform mat4 view;

void main() {
	gl_Position = projection * vec4(vertex.xy, 0, 1);
	tileColor = color; // / 255.f;
}
