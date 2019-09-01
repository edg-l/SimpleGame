#version 330 core

flat in int tileType;


void main() {
	if(tileType == 0)
		gl_FragColor = vec4(1, 0, 0, 1);
	else
		gl_FragColor = vec4(0, 1, 0, 1);
}
