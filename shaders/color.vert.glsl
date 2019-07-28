#version 450 core

layout(location = 0) in vec4 inPos;
//layout(location = 1) in mat4 inModel; -> For Instace Drawing

out vec3 worldPos;

uniform mat4 PView;
uniform mat4 Model;

uniform vec3 Camera;

void main()
{
	vec4 wp = Model * vec4(inPos.x, inPos.yzw);
	worldPos = vec3(wp);

	gl_Position = PView * wp;
	//gl_Position = wp;
}