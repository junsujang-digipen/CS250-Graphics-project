/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#pragma once
#include <string>

#include "glslshader.h"

class uniform;

class ShaderHandle
{
public:
	ShaderHandle(std::string name);
	
	std::string name;
	GLSLShader glslshader;

	void Use();
	void UnUse();

};
