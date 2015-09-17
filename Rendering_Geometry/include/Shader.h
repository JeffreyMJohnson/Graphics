#pragma once
#include <string>
#include <fstream>
#include <iostream>


#include "gl_core_4_4.h"
#include "GLFW\glfw3.h"


typedef unsigned int uint;

class Shader
{
public:
	enum UniformType
	{
		MAT4,
		VEC4,
		FLO1
	};

	const bool LoadShader(const char* vertexPath, const char* fragmentPath);
	void FreeShader();
	const uint GetProgram() { return mProgram; }
	void SetUniform(const char* name, const UniformType type, const void* value);
private:
	uint mProgram = 0;

	uint LoadSubShader(uint shaderType, const char* path);

};