#version 450 core

out vec4 fragColour;

uniform sampler2D diffuse_spite;

uniform vec3 Camera;
uniform vec4 color = vec4(1.0, 1.0, 1.0, 1.0);

in vec2 uvs;

void main()
{
	fragColour = texture(diffuse_spite, uvs);
}