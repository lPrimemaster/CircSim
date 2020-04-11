#version 450 core

in vec3 worldPos;

out vec4 fragColour;

uniform vec3 Camera;
uniform vec4 color = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
	fragColour = color;
}