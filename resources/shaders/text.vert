#version 330 core

layout (location = 0) in vec4 vertex;
out vec2 TexCoords;
uniform mat4 projection;
uniform mat4 view;
uniform int useView;

void main() {
	if(useView == 0)
		gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
	else
		gl_Position = projection * view * vec4(vertex.xy, 0.0, 1.0);
	TexCoords = vertex.zw;
};
