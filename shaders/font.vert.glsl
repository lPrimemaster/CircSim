#version 450 core
layout (location = 0) in vec4 vertex;
layout (location = 1) in vec3 color;
out vec2 TexCoords;
out vec3 textColor;

uniform mat4 PView;

void main()
{
	gl_Position = PView * vec4(vertex.xy, 0.0, 1.0);
	TexCoords = vertex.zw;
	textColor = color;
}