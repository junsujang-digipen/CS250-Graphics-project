/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#include "ToonAndFogScene.h"

#include "../GraphicEngine/Function.h"
#include "../GraphicEngine/GraphicEngine.h"
#include "../GraphicEngine/Model.h"
#include "../GraphicEngine/Uniform.h"

#include <Imgui/imgui_impl_opengl3.h>
#include <Imgui/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

#include "../GraphicEngine/DefaultMesh.h"


void ToonAndFogScene::setup()
{
	EP::Shader::MakeShaderHandle("3DTOONANDFOG");
	EP::Shader::CompileShader("3DTOONANDFOG", GL_VERTEX_SHADER, "../shaders/toonShdr.vert");
	EP::Shader::CompileShader("3DTOONANDFOG", GL_FRAGMENT_SHADER, "../shaders/toonShdr.frag");
	EP::Shader::LinkingShader("3DTOONANDFOG");

	EP::Shader::MakeShaderHandle("3DDefaultShader");
	EP::Shader::CompileShader("3DDefaultShader", GL_VERTEX_SHADER, "../shaders/3Dmodel.vert");
	EP::Shader::CompileShader("3DDefaultShader", GL_FRAGMENT_SHADER, "../shaders/3Dmodel.frag");
	EP::Shader::LinkingShader("3DDefaultShader");

	object = new Entity("taf");
	object->SetShader(EP::Shader::GetShader("3DTOONANDFOG"));
	object->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetSphere()));
	object->SetPos({ 0, 0,0 });
	object->SetColor({ 0.2f,0.2f,1.0f,1.0f });
	object->SetRotate({ 0,0,0 });

	object->SetScale({ 200,200,200 });

	object->AddUniform(new u_normalMatrix(object->GetMatrix()));
	object->AddUniform(new u_modelToWorld(object->GetMatrix()));	
	
	lightObj = new Entity("light");
	lightObj->SetShader(EP::Shader::GetShader("3DDefaultShader"));
	lightObj->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetSphere()));
	lightObj->SetPos({ 0, 300,300 });
	lightObj->SetColor({ 1.f,1.f,1.f,1.0f });
	lightObj->SetRotate({ 0,0,0 });
	lightObj->SetScale({ 50,50,50 });
	
	object->AddUniform(new u_worldToCamera());
	object->AddUniform(new u_light(const_cast<glm::vec3&>(lightObj->GetPos())));
	object->AddUniform(new u_ambient(ambient));
	object->AddUniform(new u_materialCol(materialCol));
	object->AddUniform(new fogDensity(fDensity));
	object->AddUniform(new shininess(shinvalue));
	object->AddUniform(new fogColor(fColor));
	GraphicEngine::getCamera().setPosition({ 0.f, 0.f, 500.f });
	GraphicEngine::getCamera().resetAngle();
	AddModel(object);
	AddModel(lightObj);
}

void ToonAndFogScene::update(double dt)
{
	object->Update(dt);
	lightObj->Update(dt);
}

void ToonAndFogScene::draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	Scene::draw();
}

void ToonAndFogScene::unload()
{
	Scene::unload();
}


void ToonAndFogScene::ImguiUpdate(double /*dt*/)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Translate"))
		{
			static glm::fvec3 rotate = object->GetRotate();
			static glm::vec3 pos = object->GetPos();
			static glm::vec3 scale = object->GetScale();

			ImGui::SliderFloat3("Rotate", &rotate.x, -GraphicEngine::PI, GraphicEngine::PI);
			ImGui::SliderFloat("PositionX", &pos.x, -500.f, 500.f);
			ImGui::SliderFloat("PositionY", &pos.y, -500.f, 500.f);
			ImGui::SliderFloat("PositionZ", &pos.z, -500.f, 500.f);
			ImGui::SliderFloat3("Scale", &scale.x, 20, 500);

			object->SetPos(pos);
			object->SetRotate(rotate);
			object->SetScale(scale);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Select Mesh"))
		{
			static glm::ivec2 StackSlice{ 10,10 };
			ImGui::SliderInt2("Stack And Slice", &StackSlice.x, 0, 50);
			if (ImGui::MenuItem("Sphere")) {
				object->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetSphere(StackSlice.x, StackSlice.y)));
			}
			if (ImGui::MenuItem("Cube")) {
				object->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetCube(StackSlice.x, StackSlice.y)));
			}
			if (ImGui::MenuItem("Cone")) {
				object->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetCone(StackSlice.x, StackSlice.y)));
			}
			if (ImGui::MenuItem("Cylinder")) {
				object->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetCylinder(StackSlice.x, StackSlice.y)));
			}
			if (ImGui::MenuItem("Plane")) {
				object->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetPlane(StackSlice.x, StackSlice.y)));
			}
			if (ImGui::MenuItem("Torus")) {
				object->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetTorus(StackSlice.x, StackSlice.y)));
			}
			if (ImGui::MenuItem("Tube")) {
				object->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetTube(StackSlice.x, StackSlice.y)));
			}
			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("TOON&FOG"))
		{
			static glm::vec3 lPos{ lightObj->GetPos()};
			ImGui::SliderFloat3("light", &lPos.x, -500, 500);
			lightObj->SetPos(lPos);

			ImGui::SliderFloat3("ambient", &ambient.x, 0, 1.f);
			ImGui::SliderFloat3("materialCol", &materialCol.x, 0.f, 1.f);
			ImGui::SliderFloat3("fColor", &fColor.x, 0.f, 1.f);
			ImGui::SliderFloat("fDensity", &fDensity, 0.001f, 1.f);
			ImGui::SliderFloat("shinvalue", &shinvalue, 1.f, 10.f);
			ImGui::EndMenu();
		}
		//if (ImGui::BeginMenu("PolygonMode"))
		//{
		//	//static bool pmode = false;
		//	if (ImGui::MenuItem("POINT"))
		//	{
		//		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		//	}
		//	if (ImGui::MenuItem("LINE"))
		//	{
		//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//	}
		//	if (ImGui::MenuItem("FILL"))
		//	{
		//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//	}
		//	ImGui::EndMenu();
		//}


		ImGui::EndMainMenuBar();
	}
}