#version 330 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in int TileType;

flat out int tileType;

uniform mat4 projection;
uniform mat4 view;

void main() {
	gl_Position = projection * view * vec4(vertex.xy, 0, 1);
	tileType = TileType;
}
