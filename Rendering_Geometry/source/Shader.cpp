#include "Shader.h"

const bool Shader::LoadShader(const char * vertexPath, const char * fragmentPath)
{

	uint vertex = LoadSubShader(GL_VERTEX_SHADER, vertexPath);
	uint fragment = LoadSubShader(GL_FRAGMENT_SHADER, fragmentPath);

	int success = GL_FALSE;
	mProgram = glCreateProgram();
	glAttachShader(mProgram, vertex);
	glAttachShader(mProgram, fragment);
	glLinkProgram(mProgram);
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	glGetProgramiv(mProgram, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length];
		glGetProgramInfoLog(mProgram, length, 0, log);
		std::cout << "Error linking shader program.\n" << log << std::endl;
		delete[] log;
		return false;
	}
	return true;
}

void Shader::FreeShader()
{
	glDeleteProgram(mProgram);
}

void Shader::SetUniform(const char * name, const UniformType type, const void * value)
{
	GLint location = glGetUniformLocation(mProgram, name);
	switch (type)
	{
	case MAT4:
		glUniformMatrix4fv(location, 1, false, (const GLfloat*)value);
		break;
	case VEC4:
		glUniform4fv(location, 1, (GLfloat*)value);
		break;
	case FLO1:
		glUniform1f(location, *(GLfloat*)value);
		break;
	}
}

uint Shader::LoadSubShader(uint shaderType, const char * path)
{
	std::ifstream stream(path);
	std::string contents = std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
	char* code = new char[contents.length() + 1];
	strncpy_s(code, contents.length()+ 1, contents.c_str(), contents.length());

	uint shader = glCreateShader(shaderType);

	glShaderSource(shader, 1, &code, 0);
	GLint success = GL_FALSE;
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length];
		glGetShaderInfoLog(shader, length, 0, log);
		std::cout << "Error compiling shader.\n" << log << std::endl;
		delete[] log;
	}

	delete[] code;

	return shader;
}
