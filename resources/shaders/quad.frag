#version 330 core

in vec2 TexCoords;
uniform vec4 quadColor;
uniform int useSampler;
uniform sampler2D tex;
void main() {
	if(useSampler != 0)
		gl_FragColor = texture(tex, TexCoords);
	else
		gl_FragColor = quadColor;
};
