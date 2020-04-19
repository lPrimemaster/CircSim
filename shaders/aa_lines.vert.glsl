#version 450 core

layout(location = 0) in vec4 inPos;

uniform mat4 PView;
uniform mat4 Model;

void main(void)
{
  	gl_Position = PView * Model * inPos;
}