#version 330

attribute vec4 position;
attribute vec4 color;
varying vec4 outColor;

uniform vec4 translation;
uniform vec4 scaling;
uniform mat4 projection;

void main() {
	vec4 scaled = position * scaling;
	vec4 translated = scaled + translation;
	vec4 projected = projection * translated;
	gl_Position = projected;
	outColor = color;
}