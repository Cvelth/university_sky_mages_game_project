#version 330

attribute vec4 position;
attribute vec4 color;
varying vec4 outColor;

uniform vec4 translation;
uniform vec4 scaling;
uniform mat4 projection;

void main() {
	gl_Position = projection * (position * scaling + translation);
	outColor = color;
}