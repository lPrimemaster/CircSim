#version 450 core

layout (triangles) in;
layout (line_strip, max_vertices = 3) out;

uniform float lineWidth = 0.1;
uniform mat4 PView;

void outlineTriangle(vec4 p0, vec4 p1, vec4 p2)
{
	gl_Position = p0;
	EmitVertex();
	gl_Position = p1;
	EmitVertex();
	gl_Position = p2;
	EmitVertex();
}

void main()
{
	outlineTriangle(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);
	EndPrimitive();
}