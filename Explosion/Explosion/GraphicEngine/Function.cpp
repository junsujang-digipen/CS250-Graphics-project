/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#include "Function.h"
#include "GraphicEngine.h"
#include "Shader.h"
#include "Mesh.h"
#include "Model.h"

using GE = GraphicEngine;
namespace EP //mean Explosion
{
		namespace View
	{
			void MakeViewport(std::string viewName, glm::ivec2 pos, glm::ivec2 size)
			{
				assert(ExistViewport(viewName) != true);
				GE::viewports.insert({viewName,Viewport(pos,size)});
			}
			bool ExistViewport(std::string viewName)
			{
				return GE::viewports.find(viewName) != GE::viewports.end();
			}
			void SetViewport(std::string viewName)
			{
				//auto iter = GE::viewports.find(viewName);
				//Viewport view = if(GE::viewports.find(viewName));
				//if(v)
				//view.Draw();
			}


	}
	namespace Shader
	{
		void MakeShaderHandle(std::string handleName)
		{
			if (ExistShaderHandle(handleName) == true)
			{
				return;
			}
				GE::shaders.insert({ handleName,new ShaderHandle(handleName) });
		}
			
		bool ExistShaderHandle(std::string handleName)
		{
			return GE::shaders.find(handleName) != GE::shaders.end();
		}

		ShaderHandle* GetShader(std::string name)
		{
			if(ExistShaderHandle(name))
			{
				return  GE::shaders.find(name)->second;
			}
				return nullptr;
		}

			
		void CompileShader(std::string handleName, GLenum ShaderType, std::string shaderpath)
		{
			GLSLShader& glsl = GE::shaders.find(handleName)->second->
				glslshader;
			
			glsl.CompileShaderFromFile(ShaderType, shaderpath);
		}
			
		void LinkingShader(std::string handleName)
		{
			GLSLShader& shader = GE::shaders.find(handleName)->second->glslshader;
			shader.Link();
			shader.Validate();
			shader.PrintActiveAttribs();
			shader.PrintActiveUniforms();
			if (GL_FALSE == shader.IsLinked()) {

				std::cout << "Unable to compile/link/validate shader programs\n";
				std::cout << shader.GetLog() << "\n";
				std::exit(EXIT_FAILURE);
			}
		}

	}

	namespace Mesh
	{
		void MakeMesh(std::string name)
		{
			if(ExistMesh(name) == true)
			{
				return;
			}

			GE::Meshs.insert({ name, new MeshHandle(name) });
		}

		bool ExistMesh(std::string name)
		{
			return GE::Meshs.find(name) != GE::Meshs.end();
		}

		MeshHandle* GetMesh(std::string name)
		{
			if(ExistMesh(name))
			{
				return  GE::Meshs.find(name)->second;
			}
			else
			{
				return nullptr;
			}
		}
			
		void LoadMesh(std::string , std::string )
		{
			
		}
		std::string GetTriangleMesh()
		{
			std::string meshName = "Triangle";
			if (ExistMesh(meshName))
			{
				return meshName;
			}
			else
			{
				MeshHandle* m = new MeshHandle(meshName);
				std::vector<glm::vec3> clr_vtx{ {1.f,0.f,0.f},{0.f,1.f,0.f},{0.f,0.f,1.f} };
				std::vector<glm::vec2> pos_vtx{ {0.f,0.5f},{-0.5f,-0.5f},{0.5f,-0.5f} };
				std::vector<GLuint> idx_vtx{ 0,1,2 };

				GLuint vbo_hdl;
				glCreateBuffers(1, &vbo_hdl);
				glNamedBufferStorage(vbo_hdl,
					sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec3) * clr_vtx.size(),
					nullptr, GL_DYNAMIC_STORAGE_BIT);
				glNamedBufferSubData(vbo_hdl, 0,
					sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data());
				glNamedBufferSubData(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(),
					sizeof(glm::vec3) * clr_vtx.size(), clr_vtx.data());
				GLuint vao_hdl;
				glCreateVertexArrays(1, &vao_hdl);
				glEnableVertexArrayAttrib(vao_hdl, 0);
				glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec2));
				glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, 0);
				glVertexArrayAttribBinding(vao_hdl, 0, 0);

				glEnableVertexArrayAttrib(vao_hdl, 1);
				glVertexArrayVertexBuffer(vao_hdl, 1, vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(), sizeof(glm::vec3));
				glVertexArrayAttribFormat(vao_hdl, 1, 3, GL_FLOAT, GL_FALSE, 0);
				glVertexArrayAttribBinding(vao_hdl, 1, 1);


				GLuint ebo_hdl;
				glCreateBuffers(1, &ebo_hdl);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_hdl);
				glNamedBufferStorage(ebo_hdl, sizeof(GLuint) * idx_vtx.size(),
					reinterpret_cast<GLvoid*>(idx_vtx.data()),
					GL_DYNAMIC_STORAGE_BIT);
				glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
				glBindVertexArray(0);

				m->vaoid = vao_hdl;
				m->draw_cnt = 3;
				m->primitive_type = GL_TRIANGLE_FAN;
				m->primitive_cnt = 3;
				GE::Meshs.insert({ meshName, m  });
				return meshName;
			}
		}
	}

}