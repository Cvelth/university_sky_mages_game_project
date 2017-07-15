#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec2 coordinate;
out gl_PerVertex {
	vec4 gl_Position;
};

vec2 circle[60];

void main() {
	int i = 0;
	for (float a = 0; a < 6.2830; a += 0.1047, i++)
		circle[i] = vec2(cos(a), sin(a));
		
	gl_Position = vec4(circle[gl_VertexIndex], 0.0, 1.0);
	coordinate = circle[gl_VertexIndex];
}