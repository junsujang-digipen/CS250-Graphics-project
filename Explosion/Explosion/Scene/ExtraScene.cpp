/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#include <Imgui/imgui.h>

#include "ExtraScene.h"


#include "../GraphicEngine/Function.h"
#include "../GraphicEngine/GraphicEngine.h"

#include "../GraphicEngine/DefaultMesh.h"
#include "Imgui/imgui.h"

double random(double min, double max) {
	return GraphicEngine::GetRandomNum(min,max);
}
double random(double val = 2000.) {
	return GraphicEngine::GetRandomNum(-val, val);
}
void ExtraScene::GravityBallSet()
{
	float ballSpeed{1200.f};
	for (int i = 0;i < 20;++i) {
		object = new GravityBall();
		object->SetShader(EP::Shader::GetShader(shaderName));
		object->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetSphere(20, 20)));
		object->SetPos({ random(mapSize/2.f), sponPoint, random(mapSize/2.f) });
		object->SetVel({ random(ballSpeed), 0., random(ballSpeed) });
		object->SetColor({ 1.f,.3f,0.3f,255.0f });
		object->SetRotate({ 0,0,0 });
		object->SetScale({ 60,60,60 });
		AddModel(object);
		object->AddUniform(new tUniform<glm::mat4>("ModelMatrix", object->GetMatrix()));
		object->RemoveUniform("u_modelToNDC");
		redBalls.push_back(object);

		object = new GravityBall();
		object->SetShader(EP::Shader::GetShader(shaderName));
		object->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetSphere(20, 20)));
		object->SetPos({ random(mapSize/2.f), sponPoint,random(mapSize/2.f) });
		object->SetVel({ random(ballSpeed), 0., random(ballSpeed) });
		object->SetColor({ 0.3f,0.3f,1.0f,255.0f });
		object->SetRotate({ 0,0,0 });
		object->SetScale({ 60,60,60 });
		AddModel(object);
		object->AddUniform(new tUniform<glm::mat4>("ModelMatrix", object->GetMatrix()));
		object->RemoveUniform("u_modelToNDC");
		blueBalls.push_back(object);
	}
}
void ExtraScene::PlaneSet()
{
	Plane* tempP{};
	float planeSize{ mapSize / 6.f };
	float stPlanePos{ -mapSize/2.f + planeSize/2.f };
	for (int i = 0;i < 6;++i) {
		for (int j = 0;j < 6;++j) {
			tempP = new Plane(static_cast<float>(random(800., 2500.)));
			tempP->SetShader(EP::Shader::GetShader(shaderName));
			tempP->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetCube()));
			tempP->SetPos({ stPlanePos + j * planeSize,random(0.,sponPoint -500.), stPlanePos + i * planeSize });
			tempP->SetColor({ random(0.,1.),random(0.,1.),random(0.,1.),255.0f });
			tempP->SetRotate({ 0,0,0 });
			tempP->SetScale({ planeSize,50,planeSize });
			AddModel(tempP);
			planes.push_back(tempP);
			view = GraphicEngine::getCamera().getWorldToCamera();// view
			proj = GraphicEngine::getCamera().getCameraToNDC();// proj
			tempP->AddUniform(new tUniform<glm::mat4>("ModelMatrix", tempP->GetMatrix()));
			tempP->RemoveUniform("u_modelToNDC");
		}
	}
}
void ExtraScene::lightCameraSet()
{
	glGenFramebuffers(numLight, depthMapFBO);
	glGenTextures(numLight, depthMap);
	for (int i = 0; i < numLight; ++i) {
		lightCam[i].getLightCam().setFov(GraphicEngine::PI / 6.f);
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

	lightCam[0].setPosition({ 0.,8000.,0. });
	lightCam[0].getLightCam().setRotate({-GraphicEngine::PI/2.f,0,0 });
	lightCam[0].getcameraFrustum()->SetRotate({ -GraphicEngine::PI / 2.f,0,0 });
}
void ExtraScene::lightUpdate(double dt)
{
	for (int i = 0; i < numLight; ++i) {
		lightCam[i].update(dt);
		if (viewType == 0) {


		}
		else if (viewType == 1) {
			lightCam[i].setPosition(lightCam[i].getLightCam().getCameraPos());
		}
	}
}
void ExtraScene::collisionSolve(double /*dt*/)
{
	for (auto p : planes)
	{
		if (isDetected(player, p))
		{
			glm::vec3 vel = player->GetVel();

			player->SetVel({ vel.x, p->elasticVal, vel.z });

		}
		for (auto& b : blueBalls) {
			if (isDetected(b, p))
			{
				glm::vec3 vel = b->GetVel();

				b->SetVel({ vel.x,  p->elasticVal, vel.z });

			}
		}
		for (auto& r : redBalls) {
			if (isDetected(r, p))
			{
				glm::vec3 vel = r->GetVel();

				r->SetVel({ vel.x,  p->elasticVal, vel.z });

			}
		}
	}

	//rb col
	for (auto& r : redBalls) {
		for (auto& b : blueBalls) {
			if (isDetectedCircle(r, b) == true)
			{
				glm::vec3 rvel = r->GetVel();
				glm::vec3 bvel = b->GetVel();
				//std::cout << "col" << std::endl;
				r->SetVel(bvel);
				b->SetVel(rvel);
				//r->SetShader(EP::Shader::GetShader("GeoExplosion"));
				//b->SetShader(EP::Shader::GetShader("GeoExplosion"));
			}
		}
		
	}
	//bb col
	for (auto& r : blueBalls) {
		for (auto& b : blueBalls) {
			if (r != b && isDetectedCircle(r, b) == true)
			{
				//glm::vec3 rvel = r->GetVel();
				glm::vec3 bvel = b->GetVel();
				//std::cout << "col" << std::endl;

				r->SetVel(bvel);
				b->SetVel(bvel);
				r->SetShader(EP::Shader::GetShader(shaderName));
			    b->SetShader(EP::Shader::GetShader(shaderName));
			}
		}

	}
	//rr col
	for (auto& r : redBalls) {
		for (auto& b : redBalls) {
			if (r != b && isDetectedCircle(r, b) == true)
			{
				glm::vec3 rvel = r->GetVel();
				//glm::vec3 bvel = b->GetVel();
				//std::cout << "col" << std::endl;
				r->SetVel(rvel);
				b->SetVel(rvel);
				r->SetShader(EP::Shader::GetShader(shaderName));
				b->SetShader(EP::Shader::GetShader(shaderName));
			}
		}
	}

	//pb col
	for (auto& b : blueBalls) {
		if (isDetectedCircle(player, b) == true)
		{
			if (explodeT > 0.) {
				glm::vec3 distPB{  b->GetPos() - player->GetPos() };
				//glm::vec3 lengthPB{ glm::length(distPB) };
				b->SetVel(distPB*20.f);
			}
			else {
				glm::vec3 bvel = player->GetVel();
				b->SetVel(bvel);
			}
			
		}
	}
	//pr col
	for (auto& r : redBalls) {
		if (isDetectedCircle(player, r) == true)
		{
			if (explodeT > 0.) {
				glm::vec3 distPR{ r->GetPos() - player->GetPos() };
				//glm::vec3 lengthPR{ glm::length(distPR) };
				r->SetVel(distPR*20.f);
			}
			else {
				glm::vec3 rvel = player->GetVel();
				r->SetVel(rvel);
			}
		}
	}
}
void ExtraScene::liftingBalls(double /*dt*/)
{
	glm::vec3 pos{};
	for (int i = 0; i < redBalls.size();++i) {
		pos={ redBalls[i]->GetPos() };
		if (pos.y < -500.f) {
			redBalls[i]->SetPos({ random(mapSize/2.f), sponPoint, random(mapSize/2.f) });
		}
		pos = blueBalls[i]->GetPos();
		if (pos.y < -500.f) {
			blueBalls[i]->SetPos({ random(mapSize / 2.f), sponPoint, random(mapSize / 2.f) });
		}
	}
	pos = player->GetPos();
	if (pos.y < -500.f) {
		player->SetPos({ 0., sponPoint, 0. });
	}
}
void ExtraScene::lightDrawSet()
{
	for (int i = 0; i < numLight; ++i) {
		lightCamPos[i] = lightCam[i].getLightCam().getCameraPos();

	}
	for (int i = 0; i < numLight; ++i) {
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
		//ConfigureShaderAndMatrices();
		Scene::draw();
		glCullFace(GL_BACK);
		glFlush();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	view = GraphicEngine::getCamera().getWorldToCamera();// view
	proj = GraphicEngine::getCamera().getCameraToNDC();// proj
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
}
void ExtraScene::lightCameraDraw()
{
	for (int i = 0; i < numLight; ++i) {
		lightCam[i].set_light_u_modelToNDC(proj * view);
		lightCam[i].draw();

	}
}
void ExtraScene::setup()
{
	shaderName = "3DDefaultShader";
	EP::Shader::MakeShaderHandle("3DDefaultShader");
	EP::Shader::CompileShader("3DDefaultShader", GL_VERTEX_SHADER, "../shaders/3Dmodel.vert");
	EP::Shader::CompileShader("3DDefaultShader", GL_FRAGMENT_SHADER, "../shaders/3Dmodel.frag");
	EP::Shader::LinkingShader("3DDefaultShader");
	shaderName = "Shadow";
	EP::Shader::MakeShaderHandle("Shadow");
	EP::Shader::CompileShader("Shadow", GL_VERTEX_SHADER, "../shaders/Shadow.vert");
	EP::Shader::CompileShader("Shadow", GL_FRAGMENT_SHADER, "../shaders/Shadow.frag");
	EP::Shader::LinkingShader("Shadow");
	
	EP::Shader::MakeShaderHandle("Explosion");
	EP::Shader::CompileShader("Explosion", GL_VERTEX_SHADER, "../shaders/Shadow_explosion.vert");
	EP::Shader::CompileShader("Explosion", GL_GEOMETRY_SHADER, "../shaders/Shadow_explosion.geom");
	EP::Shader::CompileShader("Explosion", GL_FRAGMENT_SHADER, "../shaders/Shadow_explosion.frag");
	EP::Shader::LinkingShader("Explosion");

	PlaneSet();
	GravityBallSet();

	player = new Player();
	object = player;
	object->SetShader(EP::Shader::GetShader(shaderName));
	object->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetSphere(100, 100)));
	object->SetPos({ 0, 2500,0 });
	object->SetColor({ 1.f,1.0f,1.0f,1.f });
	object->SetRotate({ 0,0,0 });
	object->SetScale({ 250,250,250 });
	AddModel(object);
	object->AddUniform(new tUniform<glm::mat4>("ModelMatrix", object->GetMatrix()));
	object->RemoveUniform("u_modelToNDC");
	
	GraphicEngine::getCamera().setPosition({ 0.f, 1500.f, 3500.f });
	GraphicEngine::getCamera().resetAngle();
	GraphicEngine::getCamera().setSpeed(2000.f);
	
	lightCameraSet();

	AddWorldUniform(new tUniform<glm::mat4>("ShadowMatrix", shadow));

	AddWorldUniform(new tUniform <bool>("DoShadowBehindLight", behindShadow));
	AddWorldUniform(new tUniform <glm::mat3>("LightPosition", lightCamPos));
	AddWorldUniform(new tUniform<glm::mat4>("ViewMatrix", view));
	AddWorldUniform(new tUniform<glm::mat4>("Projection", proj));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}

void ExtraScene::update(double dt)
{
	lightUpdate(dt);
	collisionSolve(dt);
	liftingBalls(dt);
	Scene::update(dt);
	GraphicEngine::GetSceneManager();
}

void ExtraScene::ImguiUpdate(double dt)
{	
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Key info"))
		{
			ImGui::Text("White ball is playable");
			ImGui::Text("Mouse right buttonclick to explode white ball");
			ImGui::Text("Arrow key to move");
			ImGui::Text("Red ball and Blue ball push each other out when they collision");
			ImGui::Text("Same color ball acts together");
			ImGui::Text("White ball can act all balls");
			ImGui::Text("You can see top view by setting");
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Setting"))
		{
			static char* currItem{};
			if (ImGui::BeginCombo("Camera choose", currItem) == true) {
				bool one{ false };
				bool two{ false };

				ImGui::Selectable("Free", &one);
				ImGui::Selectable("Ball", &two);
				//static float saveHeight{10.f};
				if (one == true) {
					viewType = 0;
					//GraphicEngine::getCamera().setCameraHeight(saveHeight);
					//saveHeight = GraphicEngine::getCamera().getheight();
					//GraphicEngine::getCamera().setDistance(GraphicEngine::getCamera().getFar()/2.f);
					//GraphicEngine::getCamera().init();
				}
				if (two == true) {
					viewType = 1;
					//GraphicEngine::getCamera().setCameraHeight(saveHeight);
					//saveHeight = GraphicEngine::getCamera().getheight();
					//GraphicEngine::getCamera().setDistance(GraphicEngine::getCamera().getFar()*.9f);
					//GraphicEngine::getCamera().init();
				}

				ImGui::EndCombo();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	if (explodeT <= 0. && ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Right) == true) {
		explodeT = explodeTMax;
		stTimeExplode = GraphicEngine::GetTime();
		player->SetShader(EP::Shader::GetShader("Explosion"));
		if (player->FindUniform("u_time"))
		{
			player->RemoveUniform("u_time");
		}
		if (player->FindUniform("u_stTime"))
		{
			player->RemoveUniform("u_stTime");
		}
		player->AddUniform(new tUniform<float>("u_stTime", stTimeExplode));
		player->AddUniform(new u_time());
		
	}
	else if (explodeT > 0.f){
		//std::cout << std::to_string(GraphicEngine::GetTime() - stTimeExplode) << std::endl;
		explodeT -= dt;
		player->SetVel(glm::vec3{0.f});
	}
	else if (explodeT < 0.) {
		explodeT = 0.;
		player->SetShader(EP::Shader::GetShader(shaderName));
		player->RemoveUniform("u_time");
		player->RemoveUniform("u_stTime");
	}
}


void ExtraScene::draw()
{
	//glUniformSubroutinesuiv();
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	lightDrawSet();

	glViewport(0, 0, GraphicEngine::windowSize.x, GraphicEngine::windowSize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, depthMap[0]);
	Scene::draw();
	//glBindTexture(GL_TEXTURE_2D, 0);
}

void ExtraScene::unload()
{
	glDisable(GL_CULL_FACE);
	for (int i = 0; i < numLight;++i) {
		lightCam[i].delLight();
	}

	Scene::unload();
	planes.clear();
	blueBalls.clear();
	redBalls.clear();
}


void ExtraScene::MoveParticle()
{
	static const unsigned limit = 100;
	for(int i = 0; i < particlePoint.size();++i)
	{
		
	}
}

void GravityBall::Update(double dt)
{
	//glm::vec3 velo = GetVel();

	//SetVel(velo * glm::vec3{ 0.95,1,0.95 });

	UpdateVelocity({ 0,-9.8,0 });
	Entity::Update(dt);
}
void Player::Update(double dt)
{
	float Pspeed{30.f};
	if (ImGui::IsKeyDown(ImGuiKey_LeftArrow) == true)
	{
		UpdateVelocity({ -Pspeed,0,0 });
	}
	if (ImGui::IsKeyDown(ImGuiKey_RightArrow) == true)
	{
		UpdateVelocity({ Pspeed,0,0 });
	}
	if (ImGui::IsKeyDown(ImGuiKey_UpArrow) == true)
	{
		UpdateVelocity({ 0,0,-Pspeed });
	}
	if (ImGui::IsKeyDown(ImGuiKey_DownArrow) == true)
	{
		UpdateVelocity({ 0,0,Pspeed });
	}
	
	{
		glm::vec3 velo = GetVel();

		SetVel(velo * glm::vec3{ 0.95,1,0.95 });
	}

	UpdateVelocity({ 0,-9.8,0 });
	Entity::Update(dt);
}