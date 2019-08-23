#include "Program.h"

Program::Program(const std::string& shaderFileName, int flags)
{
	if (flags & ADD_GEOM_SHADER)
	{
		programID = loader::loadVS_GS_FS(shaderFileName);
	}
	else
	{
		programID = loader::loadVS_FS(shaderFileName);
	}
}

Program::~Program()
{
	glDeleteProgram(programID);
}

void Program::bind() const
{
	glUseProgram(programID);
}

void Program::unbind() const
{
	glUseProgram(0);
}

GLuint Program::getID() const
{
	return programID;
}

void Program::loadInt(const char* name, int value)
{
	glUniform1i(glGetUniformLocation(programID, name), value);
}

void Program::loadUInt(const char* name, unsigned value)
{
	glUniform1ui(glGetUniformLocation(programID, name), value);
}

void Program::loadFloat(const char* name, float value)
{
	glUniform1f(glGetUniformLocation(programID, name), value);
}

void Program::loadVector2f(const char* name, const glm::vec2 value)
{
	glUniform2f(glGetUniformLocation(programID, name), value.x, value.y);
}

void Program::loadVector3f(const char* name, const glm::vec3 value)
{
	glUniform3f(glGetUniformLocation(programID, name), value.x, value.y, value.z);
}

void Program::loadVector4f(const char* name, const glm::vec4 value)
{
	glUniform4f(glGetUniformLocation(programID, name), value.x, value.y, value.z, value.w);
}

void Program::loadMatrix4f(const char* name, const glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name), 1, GL_FALSE, &value[0][0]);
}
