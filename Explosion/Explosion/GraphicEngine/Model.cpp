/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#include "Model.h"
#include "Shader.h"
#include "Mesh.h"
#include "Uniform.h"

Modeling::~Modeling()
{
	for(uniform* u : uniformList)
	{
		delete u;
	}
	uniformList.clear();
}


void Modeling::Draw()
{
	shader->Use();
	mesh->BindVertex();
	
	for(auto uniform : uniformList)
	{
		uniform->Set(shader->glslshader);
	}

	glDrawElements(mesh->primitive_type, mesh->draw_cnt, GL_UNSIGNED_INT, NULL);
	mesh->UnbindVertex();
	shader->UnUse();
}

void Modeling::DrawBinding()
{
	shader->Use();
	mesh->BindVertex();

	for (auto uniform : uniformList)
	{
		uniform->Set(shader->glslshader);
	}
}

void Modeling::DrawModel()
{
	glDrawElements(mesh->primitive_type, mesh->draw_cnt, GL_UNSIGNED_INT, NULL);
}

void Modeling::DrawUnBinding()
{
	mesh->UnbindVertex();
	shader->UnUse();
}


void Modeling::SetMesh(MeshHandle* Mesh)
{
	mesh = Mesh;
}

void Modeling::SetShader(ShaderHandle* Shader)
{
	shader = Shader;
}

void Modeling::AddUniform(uniform* uniform)
{
	uniformList.push_back(uniform);
}

bool Modeling::FindUniform(std::string Name)
{
	for (int i = 0; i < uniformList.size(); ++i)
	{
		if (uniformList[i]->name == Name)
		{
			return true;
		}
	}
	return false;
}

void Modeling::RemoveUniform(std::string Name)
{
	for (int i = 0; i < uniformList.size(); ++i)
	{
		if (uniformList[i]->name == Name)
		{
			uniform* temp = uniformList[i];
			uniformList.erase(uniformList.begin() + i);
			delete temp;
			return;
		}
	}
	
	std::cout << "Remove Uniform Error Uniform doesn't exist" << std::endl;
}

void Modeling::ClearUniform()
{
	for (uniform* u : uniformList)
	{
		delete u;
	}
	uniformList.clear();
}
