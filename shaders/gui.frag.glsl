#version 450 core
out vec4 color;

in vec4 outColor;

void main()
{
	color = outColor;
}