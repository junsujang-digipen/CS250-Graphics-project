/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#pragma once
#include <string>
#include <GL/glew.h>

class MeshHandle
{
public:
	MeshHandle(std::string name) : name(name){};
	std::string name;

	GLenum primitive_type; // which OpenGL primitive to be rendered?
	GLuint primitive_cnt{0};
	GLuint vaoid{0}; // handle to VAO
	GLuint draw_cnt{0};
	void setPrimitiveType(GLenum type) { primitive_type = type; }
	void BindVertex();
	void UnbindVertex();
};
