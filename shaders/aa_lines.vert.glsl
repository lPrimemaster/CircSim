#version 450 core

layout(location = 0) in vec4 inPos;

void main(void)
{
  gl_Position = inPos;
}