#version 330

attribute vec4 position;
attribute vec4 color;
attribute vec4 translation;
varying vec4 outColor;

uniform mat4 projection;

void main() {
	vec4 translated = position + translation;
	vec4 projected = projection * translated;
	gl_Position = projected;
	outColor = color;
}