/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#pragma once
#include "string"
#include "glslshader.h"

class uniform
{
public:
	uniform(std::string n) : name(n) {}
	virtual ~uniform() {}
	virtual void Set(GLSLShader shader) = 0;
	std::string name;
};


class u_time : public uniform
{
public:
	u_time() :  uniform("u_time") {}

	void Set(GLSLShader shader) override;
};

class u_deltatime : public uniform
{
public:
	u_deltatime();
	void Set(GLSLShader shader) override;

	double* dt;
};

class u_color : public uniform
{
public:
	u_color(glm::vec4& color);
	void Set(GLSLShader shader) override;
	
 glm::vec4& color;
};

class u_resolution : public uniform
{
public:
	u_resolution();

	void Set(GLSLShader shader) override;
};

class u_modelToNDC : public uniform
{
	glm::mat4& modelMat;
public:
	u_modelToNDC(glm::mat4& modelMat);

	void Set(GLSLShader shader) override;
	
};

class u_normalMatrix : public uniform
{
	glm::mat4& modelMat;
public:
	u_normalMatrix(glm::mat4& modelMat);

	void Set(GLSLShader shader) override;
};

class u_modelToWorld : public uniform
{
	glm::mat4& modelMat;
public:
	u_modelToWorld(glm::mat4& modelMat);
	
	void Set(GLSLShader shader) override;
};

class u_worldToCamera : public uniform
{
public:
	u_worldToCamera();
	
	void Set(GLSLShader shader) override;
};

class u_light : public uniform
{
	glm::vec3& light;
public:
	u_light(glm::vec3& light) :uniform("u_light"), light(light) {}

	void Set(GLSLShader shader) override;
};
class u_ambient : public uniform
{
	glm::vec3& ambient;
public:
	u_ambient(glm::vec3& ambient) :uniform("u_ambient"), ambient(ambient) {}

	void Set(GLSLShader shader) override;
};

class u_materialCol : public uniform
{
	glm::vec3& materialCol;
public:
	u_materialCol(glm::vec3& materialCol) :uniform("u_materialCol"), materialCol(materialCol) {}

	void Set(GLSLShader shader) override;
};
class fogDensity : public uniform
{
	float& density;
public:
	fogDensity(float& den) : uniform("fogDensity"),density(den){}

	void Set(GLSLShader shader) override;
};
class shininess : public uniform
{
	float& shin;
public:
	shininess(float& shininess) : uniform("shininess"), shin(shininess) {}

	void Set(GLSLShader shader) override;
};
class fogColor : public uniform
{

	glm::vec3& fcolor;
public:
	fogColor(glm::vec3& fcolor) : uniform("fogColor"), fcolor(fcolor){}
	void Set(GLSLShader shader) override;
};

template<typename T>
class tUniform : public uniform
{
public:
	tUniform(std::string n, T& lef) : uniform(n), value(lef){}
		~tUniform() {}
	void Set(GLSLShader shader) override{
		shader.SetUniform(name.c_str(), value);
		}
private:
	T& value;
};

