#version 450 core

layout(location = 0) in vec4 inPos;
layout(location = 1) in vec2 inUvs;
//layout(location = 1) in mat4 inModel; -> For Instace Drawing

uniform mat4 PView;
uniform mat4 Model;

out vec2 uvs;

void main()
{
	uvs = inUvs;
	gl_Position = PView * Model * inPos;
}