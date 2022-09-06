/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#include <Imgui/imgui.h>
#include "ShadowScene.h"

#include "../GraphicEngine/Function.h"
#include "../GraphicEngine/GraphicEngine.h"
#include "../GraphicEngine/Mesh.h"

#include "../GraphicEngine/DefaultMesh.h"

void ShadowScene::setup()
{
	EP::Shader::MakeShaderHandle("3DDefaultShader");
	EP::Shader::CompileShader("3DDefaultShader", GL_VERTEX_SHADER, "../shaders/3Dmodel.vert");
	EP::Shader::CompileShader("3DDefaultShader", GL_FRAGMENT_SHADER, "../shaders/3Dmodel.frag");
	EP::Shader::LinkingShader("3DDefaultShader");

	EP::Shader::MakeShaderHandle("Shadow");
	EP::Shader::CompileShader("Shadow", GL_VERTEX_SHADER, "../shaders/Shadow.vert");
	EP::Shader::CompileShader("Shadow", GL_FRAGMENT_SHADER, "../shaders/Shadow.frag");
	EP::Shader::LinkingShader("Shadow");
	//glGetSubroutineIndex();

	object = new Entity("anthing");
	object->SetShader(EP::Shader::GetShader("Shadow"));
	object->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetPlane()));
	object->SetPos({ 0, 0,0 });
	object->SetColor({ 0.5f,1.0f,0.5f,255.0f });
	object->SetRotate({ -GraphicEngine::PI / 2.f,0,0 });
	object->SetScale({ 5000,5000,5000 });
	view = GraphicEngine::getCamera().getWorldToCamera();// view
	proj = GraphicEngine::getCamera().getCameraToNDC();// proj

	object->AddUniform(new tUniform<glm::mat4>("ModelMatrix", object->GetMatrix()));

	object->RemoveUniform("u_modelToNDC");
	object->RemoveUniform("u_color");
	GraphicEngine::getCamera().setPosition({ 0.f, 300.f, 300.f });
	GraphicEngine::getCamera().resetAngle();
	AddModel(object);
	
	for (int i = 0;i < 15;++i) {
		object = new Entity("anthing10");
		object->SetShader(EP::Shader::GetShader("Shadow"));
		object->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetRandomMesh()));
		object->SetPos({ 
			GraphicEngine::GetRandomNum(-2500 ,2500.), 
			GraphicEngine::GetRandomNum(100. ,200.),
			GraphicEngine::GetRandomNum(-2500. ,2500.) });
		object->SetColor({ 
			GraphicEngine::GetRandomNum(0. ,1.f),
			GraphicEngine::GetRandomNum(0. ,1.f),
			GraphicEngine::GetRandomNum(0. ,1.f),
			255.0f });
		object->SetRotate({ 
			GraphicEngine::GetRandomNum(-GraphicEngine::PI ,GraphicEngine::PI),
			GraphicEngine::GetRandomNum(-GraphicEngine::PI ,GraphicEngine::PI),
			GraphicEngine::GetRandomNum(-GraphicEngine::PI ,GraphicEngine::PI) });
		object->SetScale({ 
			GraphicEngine::GetRandomNum(50. ,200.),
			GraphicEngine::GetRandomNum(50. ,200.),
			GraphicEngine::GetRandomNum(50. ,200.) });
		object->AddUniform(new tUniform<glm::mat4>("ModelMatrix", object->GetMatrix()));
		object->RemoveUniform("u_modelToNDC");
		object->RemoveUniform("u_color");
		AddModel(object);
	}


	object = new Entity("anth2ing");
	object->SetShader(EP::Shader::GetShader("Shadow"));
	object->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetSphere(20, 20)));
	object->SetPos({ 0, 100,0 });
	object->SetColor({ 0.5f,1.0f,0.5f,255.0f });
	object->SetRotate({ 0,0,0 });
	object->SetScale({ 50,50,50 });
	object->AddUniform(new tUniform<glm::mat4>("ModelMatrix", object->GetMatrix()));
	object->RemoveUniform("u_modelToNDC");
	object->RemoveUniform("u_color");
	AddModel(object);

	// light cam
	glGenFramebuffers(2, depthMapFBO);
	glGenTextures(2, depthMap);
	for (int i = 0;i < 2;++i) {
		lightCam[i].setup();
		lightCam[i].getLightSource()->SetShader(EP::Shader::GetShader("3DDefaultShader"));
		lightCam[i].getLightSource()->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetSphere(20, 20)));
		lightCam[i].getcameraFrustum()->SetShader(EP::Shader::GetShader("3DDefaultShader"));
		lightCam[i].getcameraFrustum()->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetFrustum(
			lightCam[i].getLightCam().getFov(),
			lightCam[i].getLightCam().getAr(),
			lightCam[i].getLightCam().getNear(),
			lightCam[i].getLightCam().getFar(),
			lightCam[i].getLightCam().getFarWidth())));
		

		
		glBindTexture(GL_TEXTURE_2D, depthMap[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			(GLsizei)shadowMapSize.x, (GLsizei)shadowMapSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &border);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap[i], 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	lightCam[0].setPosition({ 0.,300.,500. });
	lightCam[0].getLightCam().setRotate({ -0.365,0,0 });
	lightCam[0].getcameraFrustum()->SetRotate({ -0.365,0,0 });
	lightCam[1].setPosition({ 0.,300.,-500. });
	lightCam[1].getLightCam().setRotate({ -2.665,0,0 });
	lightCam[1].getcameraFrustum()->SetRotate({ -2.665,0,0 });
	//light cam and shadow map

	AddWorldUniform(new tUniform<glm::mat4>("ShadowMatrix", shadow));

	AddWorldUniform(new tUniform < bool > ("DoShadowBehindLight", behindShadow));
	AddWorldUniform(new tUniform <glm::mat3>("LightPosition", lightCamPos));
	AddWorldUniform(new tUniform<glm::mat4>("ViewMatrix", view));
	AddWorldUniform(new tUniform<glm::mat4>("Projection", proj));
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	
}

void ShadowScene::update(double dt)
{
	for (int i = 0;i < numLight;++i) {
		lightCam[i].update(dt);
		if (viewType == 0) {


		}
		else if (viewType == 1) {
			lightCam[i].setPosition(lightCam[i].getLightCam().getCameraPos());
		}
	}
	Scene::update(dt);
	//object->Update(dt);
}


void ShadowScene::draw()
{

	//glUniformSubroutinesuiv();
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	for (int i = 0;i < numLight;++i) {
		lightCamPos[i] = lightCam[i].getLightCam().getCameraPos();
		
	}
	for (int i = 0;i < numLight;++i) {
		view = lightCam[i].getLightCam().getWorldToCamera();// view
		proj = lightCam[i].getLightCam().getCameraToNDC();// proj
		shadow = shadowBias * lightCam[i].getLightCam().getCameraToNDC() * lightCam[i].getLightCam().getWorldToCamera();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, (GLsizei)shadowMapSize.x, (GLsizei)shadowMapSize.y);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[i]);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(polygon_offset, polygon_units);

		Scene::draw();
		glCullFace(GL_BACK);
		glFlush();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	if (viewType == 0) {
		view = GraphicEngine::getCamera().getWorldToCamera();// view
		proj = GraphicEngine::getCamera().getCameraToNDC();// proj
		//lightCam.getLightCam().setisMove(false);
		GraphicEngine::getCamera().setisMove(true);


	}
	else if (viewType == 1) {
		view = lightCam[0].getLightCam().getWorldToCamera();// view
		proj = lightCam[0].getLightCam().getCameraToNDC();// proj
		//lightCam.getLightCam().setisMove(true);
		GraphicEngine::getCamera().setisMove(false);
	}

	glViewport(0, 0, GraphicEngine::windowSize.x, GraphicEngine::windowSize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glBindTextures(0, numLight, depthMap);
	//glBindTexture(GL_TEXTURE_2D, depthMap);
	Scene::draw();
	for (int i = 0;i < numLight;++i) {
		lightCam[i].set_light_u_modelToNDC(proj * view);
		lightCam[i].draw();

	}

}


void ShadowScene::ImguiUpdate(double /*dt*/)
{
	if (ImGui::BeginMainMenuBar())
	{

		if (ImGui::BeginMenu("Light control"))
		{
			static glm::vec3 rotate = { -0.365f ,0,0};
			static glm::vec3 pos = lightCam[0].getLightCam().getCameraPos();

			ImGui::SliderInt("View type",&viewType,0,1);

			ImGui::SliderFloat2("Rotate", &rotate.x, -GraphicEngine::PI, GraphicEngine::PI);
			ImGui::SliderFloat("PositionX", &pos.x, -3000.f, 3000.f);
			ImGui::SliderFloat("PositionY", &pos.y, -3000.f, 3000.f);
			ImGui::SliderFloat("PositionZ", &pos.z, -3000.f, 3000.f);
			ImGui::InputFloat("Near", &lightCam[0].getLightCam().getNear(),1.);
			ImGui::InputFloat("Far", &lightCam[0].getLightCam().getFar(),1.);
			ImGui::InputFloat("Fov", &lightCam[0].getLightCam().getFov(), 0.1f);
			ImGui::InputFloat("PolygonOffset factor",&polygon_offset,1.);
			ImGui::InputFloat("PolygonOffset units",&polygon_units,1.);
			static char* currItem{};
			if (ImGui::BeginCombo("Border", currItem) == true) {
				bool one{false};
				bool two{false};

				ImGui::Selectable("Near",&one);
				ImGui::Selectable("Far", &two);
				if (one == true) {
					border = 0.f;
				}
				if (two == true) {
					border = 1.f;
				}

				ImGui::EndCombo();
			}
			ImGui::Checkbox("behindShadow", &behindShadow);


			lightCam[0].getLightCam().init();
			lightCam[0].setPosition(pos);
			lightCam[0].getLightCam().setRotate(rotate);
			lightCam[0].getcameraFrustum()->SetRotate(rotate);
			lightCam[0].getcameraFrustum()->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetFrustum(
				lightCam[0].getLightCam().getFov(),
				lightCam[0].getLightCam().getAr(),
				lightCam[0].getLightCam().getNear(),
				lightCam[0].getLightCam().getFar(),
				lightCam[0].getLightCam().getFarWidth())));

			glBindTexture(GL_TEXTURE_2D, depthMap[0]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
				(GLsizei)shadowMapSize.x, (GLsizei)shadowMapSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &border);
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[0]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap[0], 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Translate"))
		{
			static glm::fvec3 rotate = object->GetRotate();
			static glm::vec3 pos = object->GetPos();
			static glm::vec3 scale = object->GetScale();

			ImGui::SliderFloat3("Rotate", &rotate.x, -GraphicEngine::PI, GraphicEngine::PI);
			ImGui::SliderFloat("PositionX", &pos.x, -500.f, 500.f);
			ImGui::SliderFloat("PositionY", &pos.y, -500.f, 500.f);
			ImGui::SliderFloat("PositionZ", &pos.z, 0.f, 500.f);
			ImGui::SliderFloat3("Scale", &scale.x, 20, 500);

			object->SetPos(pos);
			object->SetRotate(rotate);
			object->SetScale(scale);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Select Mesh"))
		{
			static glm::ivec2 StackSlice{ 10,10 };
			//ImGui::SliderInt2("Stack And Slice", &StackSlice.x, 0, 50);
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

		if (ImGui::BeginMenu("PolygonMode"))
		{
			//static bool pmode = false;
			if (ImGui::MenuItem("POINT"))
			{
				//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
				object->SetDrawMode(GL_POINT);
			}
			if (ImGui::MenuItem("LINE"))
			{
				//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				object->SetDrawMode(GL_LINE);
			}
			if (ImGui::MenuItem("FILL"))
			{
				//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				object->SetDrawMode(GL_FILL);
			}
			ImGui::EndMenu();
		}


		ImGui::EndMainMenuBar();
	}
}

void ShadowScene::unload()
{
	glDisable(GL_CULL_FACE);
	for (int i = 0; i < numLight;++i) {
		lightCam[i].delLight();
	}
	Scene::unload();
}


