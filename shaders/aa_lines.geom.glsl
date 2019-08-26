#version 450 core

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out; //Only generating 4 vertices should not impact performance significantly

uniform float lineWidth = 0.1;
uniform mat4 PView;

//There is no passthrought function as we are not using ogl pipelines in the client
void fatlineFromLine(vec4 start, vec4 end)
{
	//Get the line direction
	vec4 direction = end - start;
	vec4 D = normalize(direction); //Assuming z and w are null
	vec4 N = vec4(D.y, -D.x, 0.0, 0.0);

	gl_Position = PView * (start + N * lineWidth / 2.0); //Start RHS
	EmitVertex();
	gl_Position = PView * (start - N * lineWidth / 2.0); //Start LHS
	EmitVertex();

	gl_Position = PView * (end + N * lineWidth / 2.0); //End RHS
	EmitVertex();
	gl_Position = PView * (end - N * lineWidth / 2.0); //End LHS
	EmitVertex();
}

void main()
{
	fatlineFromLine(gl_in[0].gl_Position, gl_in[1].gl_Position);
}