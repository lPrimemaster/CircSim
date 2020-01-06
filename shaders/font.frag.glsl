#version 450 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D tex;
uniform vec3 textColor;

void main()
{
	color = vec4(textColor, texture(tex, TexCoords).r);
}