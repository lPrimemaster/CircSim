#version 450 core
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec4 color;
//layout (location = 1) in mat4 model; -> End up using this later for performance

out vec4 outColor;

uniform mat4 proj;
uniform mat4 model;

void main()
{
	outColor = color;
	gl_Position = proj * model * vec4(vertex, 0.0, 1.0);
}