#version 330 core

in vec2 TexCoords;
uniform vec4 textColor;
uniform sampler2D text;

void main() {
	gl_FragColor = textColor * vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
};
