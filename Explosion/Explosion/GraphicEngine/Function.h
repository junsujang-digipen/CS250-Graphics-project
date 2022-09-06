/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/vec2.hpp>


//in part of Scene. we will include only this.
//mean we don't access Graphic Engine, instead we will use this Global function

class MeshHandle;
class uniform;
class Modeling;
class ShaderHandle;
namespace EP //mean Explosion
{
	namespace View
	{
		void MakeViewport(std::string viewportName,glm::ivec2 pos, glm::ivec2 size);
		void SetViewport(std::string viewportName);
		bool ExistViewport(std::string viewportName); //helper
	}
	
	namespace Shader
	{
		void MakeShaderHandle(std::string handleName);
		bool ExistShaderHandle(std::string handlepath);
		ShaderHandle* GetShader(std::string handleName);
		void CompileShader(std::string handleName, GLenum ShaderType, std::string shaderpath);
		void LinkingShader(std::string handleName); //before linking, you must Complie all shader that you need
	}

	namespace Mesh
	{
		void MakeMesh(std::string name);
		bool ExistMesh(std::string name);
		MeshHandle* GetMesh(std::string MeshName);
		void LoadMesh(std::string meshName, std::string filepath);
		
		std::string GetTriangleMesh();
	}

	namespace model
	{
	}
}
