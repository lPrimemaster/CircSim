#include "loader.h"
#include <cstdarg>
#include <fstream>
#include <sstream>

/* This section of code I use is always the same */
/* I copy it (almost everything) from all my other releases of code (e.g. other software) */

std::string getSource(const std::string& sourceFile, const std::string& type)
{
	std::ifstream infile("shaders/" + sourceFile + "." + type + ".glsl");
	std::string source;
	std::stringstream stringStream;

	if (!infile.is_open())
	{
		std::cerr << "Couldn't open shader source: " + sourceFile << std::endl;
		throw std::runtime_error("Couldn't open shader source: " + sourceFile);
	}

	stringStream << infile.rdbuf();
	source = stringStream.str();

	return source;
}

GLuint compileShader(const GLchar* source, GLenum type)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	GLint status;
	GLchar infolog[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		glGetShaderInfoLog(shader, 512, nullptr, infolog);
		std::cerr << "Compiling shader failed: " + std::string(infolog) << std::endl;
		throw std::runtime_error("Error compiling shader: " + std::string(infolog));
	}

	return shader;
}

GLuint createProgram(size_t argsize, ...)
{
	va_list ap;
	va_start(ap, argsize);

	GLuint program = glCreateProgram();

	GLuint shader = va_arg(ap, GLuint);
	glAttachShader(program, shader);

	for (int i = 1; i < argsize; i++)
	{
		GLuint ishader = va_arg(ap, GLuint);
		glAttachShader(program, ishader);
	}

	glLinkProgram(program);

	GLint status;
	GLchar infolog[512];

	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status)
	{
		glGetProgramInfoLog(program, 512, nullptr, infolog);
		std::cerr << "Linking program failed: " + std::string(infolog) << std::endl;
		throw std::runtime_error("Error linking program: " + std::string(infolog));
	}

	va_end(ap);

	return program;
}

GLuint loader::loadVS_FS(const std::string& name)
{
	std::string vSource, fSource;
	try
	{
		vSource = getSource(name, "vert");
		fSource = getSource(name, "frag");
	}
	catch (std::runtime_error & e)
	{
		std::cerr << e.what() << std::endl;
		std::cout << e.what() << std::endl;
	}

	GLuint vsID, fsID;
	try
	{
		vsID = compileShader(vSource.c_str(), GL_VERTEX_SHADER);
		fsID = compileShader(fSource.c_str(), GL_FRAGMENT_SHADER);
	}
	catch (std::runtime_error & e)
	{
		std::cerr << e.what() << std::endl;
		std::cout << e.what() << std::endl;
		exit(-10);
	}
	GLuint programID;
	try
	{
		programID = createProgram(2, vsID, fsID);
	}
	catch (std::runtime_error & e)
	{
		std::cerr << e.what() << std::endl;
		std::cout << e.what() << std::endl;
		exit(-10);
	}

	glDeleteShader(vsID);
	glDeleteShader(fsID);

	return programID;
}

GLuint loader::loadVS_GS_FS(const std::string& name)
{
	std::string vSource, gSource, fSource;
	try
	{
		vSource = getSource(name, "vert");
		gSource = getSource(name, "geom");
		fSource = getSource(name, "frag");
	}
	catch (std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
		std::cout << e.what() << std::endl;
	}

	GLuint vsID, gsID, fsID;
	try
	{
		vsID = compileShader(vSource.c_str(), GL_VERTEX_SHADER);
		gsID = compileShader(gSource.c_str(), GL_GEOMETRY_SHADER);
		fsID = compileShader(fSource.c_str(), GL_FRAGMENT_SHADER);
	}
	catch (std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
		std::cout << e.what() << std::endl;
		exit(-10);
	}
	GLuint programID;
	try
	{
		programID = createProgram(3, vsID, gsID, fsID);
	}
	catch (std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
		std::cout << e.what() << std::endl;
		exit(-10);
	}

	glDeleteShader(vsID);
	glDeleteShader(gsID);
	glDeleteShader(fsID);

	return programID;
}
