#version 330

attribute vec4 position;
attribute vec4 color;
varying vec4 theColor;

uniform vec2 offsets[@];

void main() {
	vec2 offset = offsets[gl_InstanceID];
	gl_Position = position + vec4(offset, 0.f, 1.f);
	theColor = color;
}