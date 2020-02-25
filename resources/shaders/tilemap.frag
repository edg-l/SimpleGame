#version 330 core

in vec4 tileColor;

void main() {
	gl_FragColor = (1.f / 255.f) * tileColor;
}
