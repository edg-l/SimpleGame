#version 330 core

in vec4 tileColor;

void main() {
	gl_FragColor = tileColor / 255.f;
}
