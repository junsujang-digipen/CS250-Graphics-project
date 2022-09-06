/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

#include <Imgui/imgui.h>

#include "tessScene.h"

#include "../GraphicEngine/Function.h"
#include "../GraphicEngine/GraphicEngine.h"

#include "../GraphicEngine/DefaultMesh.h"
#include "Imgui/imgui.h"
#include "../GraphicEngine/Mesh.h"
void TessScene::setup()
{
	EP::Shader::MakeShaderHandle("TessTestShdr");
	EP::Shader::CompileShader("TessTestShdr", GL_VERTEX_SHADER, "../shaders/tessTestShdr.vert");
	EP::Shader::CompileShader("TessTestShdr", GL_TESS_CONTROL_SHADER, "../shaders/tessTestShdr.tesc");
	EP::Shader::CompileShader("TessTestShdr", GL_TESS_EVALUATION_SHADER, "../shaders/tessTestShdr.tese");
	EP::Shader::CompileShader("TessTestShdr", GL_GEOMETRY_SHADER, "../shaders/tessTestShdr.geom");
	EP::Shader::CompileShader("TessTestShdr", GL_FRAGMENT_SHADER, "../shaders/tessTestShdr.frag");
	EP::Shader::LinkingShader("TessTestShdr");

	EP::Shader::MakeShaderHandle("TessTestShdr2");
	EP::Shader::CompileShader("TessTestShdr2", GL_VERTEX_SHADER, "../shaders/tessTestShdr.vert");
	EP::Shader::CompileShader("TessTestShdr2", GL_TESS_CONTROL_SHADER, "../shaders/tessTestShdr2.tesc");
	EP::Shader::CompileShader("TessTestShdr2", GL_TESS_EVALUATION_SHADER, "../shaders/tessTestShdr2.tese");
	EP::Shader::CompileShader("TessTestShdr2", GL_GEOMETRY_SHADER, "../shaders/tessTestShdr.geom");
	EP::Shader::CompileShader("TessTestShdr2", GL_FRAGMENT_SHADER, "../shaders/tessTestShdr.frag");
	EP::Shader::LinkingShader("TessTestShdr2");

	EP::Shader::MakeShaderHandle("GrassShdr");
	EP::Shader::CompileShader("GrassShdr", GL_VERTEX_SHADER, "../shaders/tessTestShdr.vert");
	EP::Shader::CompileShader("GrassShdr", GL_TESS_CONTROL_SHADER, "../shaders/tessTestShdr.tesc");
	EP::Shader::CompileShader("GrassShdr", GL_TESS_EVALUATION_SHADER, "../shaders/tessTestShdr.tese");
	EP::Shader::CompileShader("GrassShdr", GL_GEOMETRY_SHADER, "../shaders/tessAndGrassShdr.geom");
	EP::Shader::CompileShader("GrassShdr", GL_FRAGMENT_SHADER, "../shaders/tessTestShdr.frag");
	EP::Shader::LinkingShader("GrassShdr");

	EP::Shader::MakeShaderHandle("3DDefaultShader");
	EP::Shader::CompileShader("3DDefaultShader", GL_VERTEX_SHADER, "../shaders/3Dmodel.vert");
	EP::Shader::CompileShader("3DDefaultShader", GL_FRAGMENT_SHADER, "../shaders/3Dmodel.frag");
	EP::Shader::LinkingShader("3DDefaultShader");

	//EP::Shader::LinkingShader("GeoShader");

	object = new Entity("anthing");
	object->SetShader(EP::Shader::GetShader("TessTestShdr"));
	object->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetSphere()));
	object->SetPos({ 0, 0,0 });
	object->SetColor({ 0.5f,1.0f,0.5f,255.0f });
	object->SetRotate({ 90,0,0 });
	
	object->getMeshHandle()->setPrimitiveType(GL_PATCHES);

	lightObj = new Entity("light");
	lightObj->SetShader(EP::Shader::GetShader("3DDefaultShader"));
	lightObj->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetSphere()));
	lightObj->SetPos({ 0, 300,300 });
	lightObj->SetColor({ 1.f,1.f,1.f,1.0f });
	lightObj->SetRotate({ 0,0,0 });
	lightObj->SetScale({ 50,50,50 });


	object->AddUniform(new u_time());//?? 
	object->AddUniform(new tUniform<int>("TessLevelInner",tessInner));
	object->AddUniform(new tUniform<int>("TessLevelOuter", tessOuter));
	object->AddUniform(new tUniform<float>("Shrink", shrink));
	object->AddUniform(new tUniform<bool>("isCircle", isCircle));
	
	object->AddUniform(new tUniform<glm::vec3>("u_windVec", windVec));
	object->RemoveUniform("u_windVec");
	object->AddUniform(new u_normalMatrix(object->GetMatrix()));
	
	object->AddUniform(new u_light(const_cast<glm::vec3&>(lightObj->GetPos())));
	object->AddUniform(new u_ambient(ambient));
	object->AddUniform(new u_materialCol(materialCol));
	object->AddUniform(new shininess(shinvalue));

	object->SetScale({ 200,200,200 });
	GraphicEngine::getCamera().setPosition({ 0.f, 0.f, 500.f });
	GraphicEngine::getCamera().resetAngle();
	AddModel(object);
}

void TessScene::update(double dt)
{
	object->Update(dt);
	//std::cout << GraphicEngine::GetTime() << std::endl;
	
}

void TessScene::ImguiUpdate(double /*dt*/)
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
		if (ImGui::BeginMenu("Tessellation"))
		{
			ImGui::SliderInt("TessLevelInner", &tessInner, 1,64);
			ImGui::SliderInt("TessLevelOuter", &tessOuter, 1,64);
			ImGui::SliderFloat("Shrink", &shrink, 0.f, 1.f);
			ImGui::SliderInt("ShaderType", &shaderType, 0, 2);
			ImGui::Checkbox("isCircle", &isCircle);
			if (shaderType == 0)
			{
				object->SetShader(EP::Shader::GetShader("TessTestShdr"));
				
			}
			else if(shaderType == 1)
			{
				object->SetShader(EP::Shader::GetShader("TessTestShdr2"));
				
			}
			else {
				ImGui::SliderFloat3("Wind vector", &windVec.x, -1.f, 1.f);
				object->SetShader(EP::Shader::GetShader("GrassShdr"));
			}

			if(shaderType == 2)
			{
				if(object->FindUniform("u_windVec") == false)
				{
					object->AddUniform(new tUniform<glm::vec3>("u_windVec", windVec));
				}
				if(object->FindUniform("isCircle") == false)
				{
					object->AddUniform(new tUniform<bool>("isCircle", isCircle));
				}
			}
			else if(shaderType == 1)
			{
				if (object->FindUniform("u_windVec") == true)
				{
					object->RemoveUniform("u_windVec");
				}
				if (object->FindUniform("isCircle") == true)
				{
					object->RemoveUniform("isCircle");
				}
			}
			else
			{
				if (object->FindUniform("u_windVec") == true)
				{
					object->RemoveUniform("u_windVec");
				}
				if (object->FindUniform("isCircle") == false)
				{
					object->AddUniform(new tUniform<bool>("isCircle", isCircle));
				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Light"))
		{
			ImGui::SliderFloat3("Light position", &const_cast<glm::vec3&>(lightObj->GetPos()).x,-500.f,500.f);
			ImGui::SliderFloat3("Ambient color", &ambient.x, 0.f, 1.f);
			ImGui::SliderFloat3("Material color", &materialCol.x, 0.f, 1.f);
			ImGui::SliderFloat("Shiness", &shinvalue, 0.f, 10.f);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Select Mesh"))
		{
			object->getMeshHandle()->setPrimitiveType(GL_TRIANGLES);
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
			object->getMeshHandle()->setPrimitiveType(GL_PATCHES);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}


void TessScene::draw()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glPatchParameteri(GL_PATCH_VERTICES,3);
	Scene::draw();

}

void TessScene::unload()
{
	object->getMeshHandle()->setPrimitiveType(GL_TRIANGLES);
	Scene::unload();
}


