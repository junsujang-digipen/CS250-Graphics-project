/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#include "Shader.h"
#include "Uniform.h"

ShaderHandle::ShaderHandle(std::string name)
	: name(name) {}


void ShaderHandle::Use()
{
	glslshader.Use();
}

void ShaderHandle::UnUse()
{

	glslshader.UnUse();
}

