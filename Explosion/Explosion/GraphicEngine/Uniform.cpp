/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#include "Uniform.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "GraphicEngine.h"
#include "glslshader.h"


void u_time::Set(GLSLShader shader)
{
	float time = GraphicEngine::GetTime();
	shader.SetUniform(name.c_str(), time);
}


u_deltatime::u_deltatime() : uniform("u_deltatime")
{
	
}

void u_deltatime::Set(GLSLShader shader)
{
	shader.SetUniform(name.c_str(), (float)*dt);
}

u_color::u_color(glm::vec4& color) : uniform("u_color"), color(color)
{}
void u_color::Set(GLSLShader shader)
{
	shader.SetUniform(name.c_str(), color);
}

u_resolution::u_resolution() : uniform("u_resolution")
{}

void u_resolution::Set(GLSLShader shader)
{
	glm::vec2 size = GraphicEngine::windowSize;
	shader.SetUniform(name.c_str(),size);
}

u_modelToNDC::u_modelToNDC(glm::mat4& modelMat)
	: uniform("u_modelToNDC"), modelMat(modelMat) {}


void u_modelToNDC::Set(GLSLShader shader)
{
	glm::mat4 wTcTn{GraphicEngine::getCamera().getWorldToCameraToNDC()*modelMat};
	shader.SetUniform(name.c_str(), wTcTn);
}

u_normalMatrix::u_normalMatrix(glm::mat4& modelMat) : uniform("u_normalMatrix"), modelMat(modelMat)
{
}

void u_normalMatrix::Set(GLSLShader shader)
{
	
	glm::mat3 u_normal = glm::transpose( glm::inverse(GraphicEngine::getCamera().getWorldToCamera() * modelMat));
	shader.SetUniform(name.c_str(), u_normal);
}

u_modelToWorld::u_modelToWorld(glm::mat4& modelMat) : uniform("u_modelToWorld"), modelMat(modelMat)
{
}
void u_modelToWorld::Set(GLSLShader shader)
{
	shader.SetUniform(name.c_str(), modelMat);
}

u_worldToCamera::u_worldToCamera() : uniform("u_worldToCamera")
{
	
}

void u_worldToCamera::Set(GLSLShader shader)
{
	glm::mat4 wtc = GraphicEngine::getCamera().getWorldToCamera();
	shader.SetUniform(name.c_str(), wtc);
}

void u_light::Set(GLSLShader shader)
{
	shader.SetUniform(name.c_str(), light);
}

void u_ambient::Set(GLSLShader shader)
{
	shader.SetUniform(name.c_str(), ambient);
}

void u_materialCol::Set(GLSLShader shader)
{
	shader.SetUniform(name.c_str(), materialCol);
}
void fogDensity::Set(GLSLShader shader)
{
	shader.SetUniform(name.c_str(), density);
}
void shininess::Set(GLSLShader shader)
{
	shader.SetUniform(name.c_str(), shin);
}

void fogColor::Set(GLSLShader shader)
{
	shader.SetUniform(name.c_str(), fcolor);
}

