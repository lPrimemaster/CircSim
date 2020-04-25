#version 450 core
in vec2 TexCoords;
in vec3 textColor;
out vec4 color;

uniform sampler2D tex;

void main()
{
	color = vec4(textColor, texture(tex, TexCoords).r);
}