#version 450 core

uniform vec4 color = vec4(1.0, 1.0, 1.0, 1.0);

out vec4 fragColor;

void main(void)
{
	fragColor = color;
}