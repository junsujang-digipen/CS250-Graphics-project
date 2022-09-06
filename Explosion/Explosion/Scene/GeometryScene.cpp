/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

#include <Imgui/imgui.h>

#include "GeometryScene.h"

#include "../GraphicEngine/Function.h"
#include "../GraphicEngine/GraphicEngine.h"

#include "../GraphicEngine/DefaultMesh.h"
#include "Imgui/imgui.h"
void GeometryScene::setup()
{
	EP::Shader::MakeShaderHandle("GeoShader");
	EP::Shader::CompileShader("GeoShader", GL_VERTEX_SHADER, "../shaders/GeoTestShdr.vert");
	EP::Shader::CompileShader("GeoShader", GL_GEOMETRY_SHADER, "../shaders/GeoTestShdr.geom");
	EP::Shader::CompileShader("GeoShader", GL_FRAGMENT_SHADER, "../shaders/GeoTestShdr.frag");
	EP::Shader::LinkingShader("GeoShader");

	EP::Shader::MakeShaderHandle("GeoSmokeShader");
	EP::Shader::CompileShader("GeoSmokeShader", GL_VERTEX_SHADER, "../shaders/GeoSmokeShdr.vert");
	EP::Shader::CompileShader("GeoSmokeShader", GL_GEOMETRY_SHADER, "../shaders/GeoSmokeShdr.geom");
	EP::Shader::CompileShader("GeoSmokeShader", GL_FRAGMENT_SHADER, "../shaders/GeoSmokeShdr.frag");
	EP::Shader::LinkingShader("GeoSmokeShader");

	EP::Shader::MakeShaderHandle("GeoExplosion");
	EP::Shader::CompileShader("GeoExplosion", GL_VERTEX_SHADER, "../shaders/GeoExplosion.vert");
	EP::Shader::CompileShader("GeoExplosion", GL_GEOMETRY_SHADER, "../shaders/GeoExplosion.geom");
	EP::Shader::CompileShader("GeoExplosion", GL_FRAGMENT_SHADER, "../shaders/GeoExplosion.frag");
	EP::Shader::LinkingShader("GeoExplosion");

	//EP::Shader::LinkingShader("GeoShader");

	object = new Entity("anthing");
	object->SetShader(EP::Shader::GetShader("GeoSmokeShader"));
	object->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetSphere(20, 20)));
	object->SetPos({ 0, 0,0 });
	object->SetColor({ 0.5f,1.0f,0.5f,255.0f });
	object->SetRotate({ 0,0,0 });
	AddWorldUniform(new u_time());
	object->SetScale({ 200,200,200 });
	GraphicEngine::getCamera().setPosition({ 0.f, 0.f, 500.f });
	GraphicEngine::getCamera().resetAngle();
	AddModel(object);
}

void GeometryScene::update(double dt)
{
	object->Update(dt);
}

void GeometryScene::ImguiUpdate(double /*dt*/)
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

		if (ImGui::BeginMenu("Shader"))
		{
			//static bool pmode = false;
			if (ImGui::MenuItem("Geometry shader"))
			{
				//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
				object->SetShader(EP::Shader::GetShader("GeoShader"));
			}
			if (ImGui::MenuItem("Smoke shader"))
			{
				//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				object->SetShader(EP::Shader::GetShader("GeoSmokeShader"));
			}
			if (ImGui::MenuItem("Explosion"))
			{
				//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				object->SetShader(EP::Shader::GetShader("GeoExplosion"));
			}
			ImGui::EndMenu();
		}


		ImGui::EndMainMenuBar();
	}
}


void GeometryScene::draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	Scene::draw();

}

void GeometryScene::unload()
{
	Scene::unload();
}


