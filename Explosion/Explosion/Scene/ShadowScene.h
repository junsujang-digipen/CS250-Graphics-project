/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#pragma once
#include "Scene.h"
#include "../src/Entity.h"
#include "../GraphicEngine/Camera.h"
class Modeling;


class LightObj {
	Camera lightCam{};
	Entity* lightSource{};
	Entity* cameraObj{};
	glm::vec3 position{};
	glm::mat4 light_u_modelToNDC{};
	//std::vector<Entity*> extraObjs{};
	void PosChange() { 
		lightCam.setPosition(position);
		lightSource->SetPos(position);
		cameraObj->SetPos(position);
	}
public:
	LightObj() {}
	void setup() {
		lightSource = new Entity("light source");
		//lightSource->SetMesh(EP::Mesh::GetMesh(EP::Mesh::GetSphere(20, 20)));
		lightSource->SetPos(position);
		lightSource->SetColor({ 1.f,1.0f,1.f,255.0f });
		lightSource->SetRotate({ 0,0,0 });
		lightSource->SetScale({ 1,1,1 });
		lightSource->RemoveUniform("u_modelToNDC");
		lightSource->AddUniform(new tUniform<glm::mat4>("u_modelToNDC", light_u_modelToNDC));

		cameraObj = new Entity("camera frustum");
		cameraObj->SetPos(position);
		cameraObj->SetColor({ 1.f,1.0f,1.f,255.0f });
		cameraObj->SetRotate({ 0,0,0 });
		cameraObj->SetScale({ 1,1,1 });
		cameraObj->RemoveUniform("u_modelToNDC");
		cameraObj->AddUniform(new tUniform<glm::mat4>("u_modelToNDC", light_u_modelToNDC));
		

		lightCam.init();
		//lightCam.setNear(1);
		lightCam.setisMove(false);
		lightCam.setPosition(position);
		lightCam.Update(0.01);
		PosChange();

	}
	void update(double dt) {
		lightSource->Update(dt);
		lightCam.Update(dt);
		cameraObj->Update(dt);
	}
	void draw() {
		lightSource->Draw();
		cameraObj->Draw();
	}
	void setPosition(glm::vec3 pos) { 
		position = pos;
		PosChange(); 
	}
	Entity* getLightSource() { return lightSource; }
	Entity* getcameraFrustum() { return cameraObj; }
	Camera& getLightCam() { return lightCam; }
	void set_light_u_modelToNDC(glm::mat4 mat) { light_u_modelToNDC = mat* cameraObj->GetMatrix(); }
	void delLight() {
		delete lightSource;
		lightSource = nullptr;
		delete cameraObj;
		cameraObj = nullptr;
	}
};

class ShadowScene : public Scene
{
public:
	ShadowScene(SceneManager& scm) : Scene(scm) {}

	void setup() override;
	void update(double dt) override;
	void ImguiUpdate(double dt) override;
	void draw() override;
	void unload() override;
	//void SetUpShadowFrameBuffer();
	glm::mat4 proj;
	glm::mat4 view;
	glm::mat4 shadow;
	Entity* object;
	glm::mat4 shadowBias = { glm::vec4(0.5f,0.f,0.f,0.f),glm::vec4(0.f,0.5f,0.f,0.f),
	glm::vec4(0.f,0.f,0.5f,0.f) ,glm::vec4(0.5f,0.5f,0.5f,1.f) };
	glm::vec2 shadowMapSize{ 1024,1024 };

	const int numLight{1};
	LightObj lightCam[2];
	unsigned int depthMapFBO[2]{};
	unsigned int depthMap[2]{};
	glm::mat3 lightCamPos{};
	
	bool behindShadow = true;
	int viewType{0};
	float polygon_offset{};
	float polygon_units{};
	float border{0.f};
};