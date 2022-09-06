/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/vec3.hpp>
//in part of Scene. we will include only this.
//mean we don't access Graphic Engine, instead we will use this Global function


namespace EP //mean Explosion
{
	namespace Mesh
	{
		void computePlane(int stacks, int slices,
			std::vector<glm::vec3>& pos_vtx,
			std::vector<glm::vec3>& nrm_vtx,
			std::vector<glm::vec2>& uv_vtx);
		void BuildIndexBuffer(int stacks, int slices, std::vector<GLuint>& idx_vtx, const std::vector<glm::vec3>& pos_vtx);
		void binding(GLuint& vao_hdl, std::vector<glm::vec3>& pos_vtx, std::vector<GLuint>& idx_vtx);
		void binding(GLuint& vao_hdl, std::vector<glm::vec3>& pos_vtx, std::vector<glm::vec3>& nrm_vtx, std::vector<GLuint>& idx_vtx);
		void binding(GLuint& vao_hdl, std::vector<glm::vec3>& pos_vtx, std::vector<glm::vec3>& nrm_vtx, std::vector<glm::vec2>& uv_vtx, std::vector<GLuint>& idx_vtx);

		std::string GetSphere(int stacks = 20, int slices = 20);
		//std::string GetIcoSphere(int stacks, int slices);
		std::string GetCube(int stacks = 10, int slices = 10);
		std::string GetCone(int stacks = 20, int slices = 20);
		std::string GetCylinder(int stacks = 20, int slices = 20);
		std::string GetPlane(int stacks = 1, int slices = 1);
		std::string GetTorus(int stacks = 20, int slices = 20, float startAngle = 0.0f, float endAngle = 3.14f * 2.f);
		std::string GetTube(int stacks = 20, int slices = 20,float thickness = 0.1f);	
		std::string GetFrustum(float fov = 0.2f,float ar = 1.5f,float near = 1.f,float far = 20.f,float width = 50.f);
		std::string GetRandomMesh();
	}
}
