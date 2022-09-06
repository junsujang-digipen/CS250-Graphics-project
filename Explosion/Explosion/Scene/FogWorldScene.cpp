/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#include "FogWorldScene.h"

#include "../GraphicEngine/Function.h"
#include "../GraphicEngine/GraphicEngine.h"
#include "../GraphicEngine/Model.h"
#include "../GraphicEngine/Uniform.h"

#include <Imgui/imgui_impl_opengl3.h>
#include <Imgui/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

#include "../GraphicEngine/DefaultMesh.h"


void FogWorldScene::setup()
{
	EP::Shader::MakeShaderHandle("3DTOONANDFOG");
	EP::Shader::CompileShader("3DTOONANDFOG", GL_VERTEX_SHADER, "../shaders/toonShdr.vert");
	EP::Shader::CompileShader("3DTOONANDFOG", GL_FRAGMENT_SHADER, "../shaders/toonShdr.frag");
	EP::Shader::LinkingShader("3DTOONANDFOG");

	EP::Shader::MakeShaderHandle("3DDefaultShader");
	EP::Shader::CompileShader("3DDefaultShader", GL_VERTEX_SHADER, "../shaders/3Dmodel.vert");
	EP::Shader::CompileShader("3DDefaultShader", GL_FRAGMENT_SHADER, "../shaders/3Dmodel.frag");
	EP::Shader::LinkingShader("3DDefaultShader");

	lightObj = new Entity("light");
	lightObj->SetShader(EP::Shader::GetShader("3DDefaultShader"));
	lightObj->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetSphere()));
	lightObj->SetPos({ 0, 300,300 });
	lightObj->SetColor({ 1.f,1.f,1.f,1.0f });
	lightObj->SetRotate({ 0,0,0 });
	lightObj->SetScale({ 50,50,50 });

	for (int i = 0;i < 15;++i) {
		
		object = new Entity("taf");
		object->SetShader(EP::Shader::GetShader("3DTOONANDFOG"));
		object->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetSphere()));
		object->SetPos({ GraphicEngine::GetRandomNum(-1000.,1000), GraphicEngine::GetRandomNum(-1000.,1000),GraphicEngine::GetRandomNum(-1000.,1000) });
		object->SetColor({ GraphicEngine::GetRandomNum(0.,1.),GraphicEngine::GetRandomNum(0.,1.),GraphicEngine::GetRandomNum(0.,1.),1.0f });
		object->SetRotate({ GraphicEngine::GetRandomNum(0.,3.14),GraphicEngine::GetRandomNum(0.,3.14),GraphicEngine::GetRandomNum(0.,3.14) });
		double sc = GraphicEngine::GetRandomNum(50., 300.);
		object->SetScale({ sc,sc,sc });

		object->AddUniform(new u_normalMatrix(object->GetMatrix()));
		object->AddUniform(new u_modelToWorld(object->GetMatrix()));

		object->AddUniform(new u_worldToCamera());
		object->AddUniform(new u_light(const_cast<glm::vec3&>(lightObj->GetPos())));
		object->AddUniform(new u_ambient(ambient));
		object->AddUniform(new u_materialCol(materialCol));
		object->AddUniform(new fogDensity(fDensity));
		object->AddUniform(new shininess(shinvalue));
		object->AddUniform(new fogColor(fColor));
		AddModel(object);

	}

	object = (new Entity("x_axis"));
	object->SetShader(EP::Shader::GetShader("3DTOONANDFOG"));
	object->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetPlane(100, 100)));
	object->SetPos({ 0, 0,0 });
	object->SetColor({ 1.f,0.f,0.f,0.1f });
	object->SetScale({ 10000,10000,10000 });
	object->SetRotate({ 0,PI * 0.5f,0 });
	object->SetDrawMode(GL_LINE);
	object->AddUniform(new u_normalMatrix(object->GetMatrix()));
	object->AddUniform(new u_modelToWorld(object->GetMatrix()));
	object->AddUniform(new u_worldToCamera());
	object->AddUniform(new u_light(const_cast<glm::vec3&>(lightObj->GetPos())));
	object->AddUniform(new u_ambient(ambient));
	object->AddUniform(new u_materialCol(materialCol));
	object->AddUniform(new fogDensity(fDensity));
	object->AddUniform(new shininess(shinvalue));
	object->AddUniform(new fogColor(fColor));
	AddModel(object);

	object = (new Entity("y_axis"));
	object->SetShader(EP::Shader::GetShader("3DTOONANDFOG"));
	object->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetPlane(100, 100)));
	object->SetPos({ 0, 0,0 });
	object->SetColor({ 0.f,0.f,1.f,0.1f });
	object->SetScale({ 10000,10000,10000 });
	object->SetRotate({ 0,0,PI * 0.5f });
	object->SetDrawMode(GL_LINE);
	object->AddUniform(new u_normalMatrix(object->GetMatrix()));
	object->AddUniform(new u_modelToWorld(object->GetMatrix()));
	object->AddUniform(new u_worldToCamera());
	object->AddUniform(new u_light(const_cast<glm::vec3&>(lightObj->GetPos())));
	object->AddUniform(new u_ambient(ambient));
	object->AddUniform(new u_materialCol(materialCol));
	object->AddUniform(new fogDensity(fDensity));
	object->AddUniform(new shininess(shinvalue));
	object->AddUniform(new fogColor(fColor));
	AddModel(object);

	object = (new Entity("z_axis"));
	object->SetShader(EP::Shader::GetShader("3DTOONANDFOG"));
	object->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetPlane(100, 100)));
	object->SetPos({ 0, 0,0 });
	object->SetColor({ 0.f,1.f,0.f,0.1f });
	object->SetScale({ 10000,10000,10000 });
	object->SetRotate({ PI * 0.5f,0,0 });
	object->SetDrawMode(GL_LINE);
	object->AddUniform(new u_normalMatrix(object->GetMatrix()));
	object->AddUniform(new u_modelToWorld(object->GetMatrix()));
	object->AddUniform(new u_worldToCamera());
	object->AddUniform(new u_light(const_cast<glm::vec3&>(lightObj->GetPos())));
	object->AddUniform(new u_ambient(ambient));
	object->AddUniform(new u_materialCol(materialCol));
	object->AddUniform(new fogDensity(fDensity));
	object->AddUniform(new shininess(shinvalue));
	object->AddUniform(new fogColor(fColor));
	AddModel(object);
	
	GraphicEngine::getCamera().setPosition({ 0.f, 0.f, 500.f });
	GraphicEngine::getCamera().resetAngle();
	
	AddModel(lightObj);
}

void FogWorldScene::update(double dt)
{
	Scene::update(dt);
}

void FogWorldScene::draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	Scene::draw();
}

void FogWorldScene::unload()
{
	Scene::unload();
}


void FogWorldScene::ImguiUpdate(double /*dt*/)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Light&FOG"))
		{
			static glm::vec3 lPos{ lightObj->GetPos() };
			ImGui::SliderFloat3("light", &lPos.x, -500, 500);
			lightObj->SetPos(lPos);

			ImGui::SliderFloat3("ambient", &ambient.x, 0, 1.f);
			ImGui::SliderFloat3("materialCol", &materialCol.x, 0.f, 1.f);
			ImGui::SliderFloat3("fColor", &fColor.x, 0.f, 1.f);
			ImGui::SliderFloat("fDensity", &fDensity, 0.001f, 0.05f);
			ImGui::SliderFloat("shinvalue", &shinvalue, 1.f, 10.f);
			ImGui::EndMenu();
		}


		ImGui::EndMainMenuBar();
	}
}