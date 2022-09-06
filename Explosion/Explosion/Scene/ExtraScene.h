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
#include "ShadowScene.h"
class Camera;
class Modeling;

class GravityBall : public Entity
{
public:
	GravityBall() : Entity("ball") {}

	void Update(double dt) override;
};
class Player : public Entity
{
public:
	Player() : Entity("player") {}

	void Update(double dt) override;
};
class Plane : public Entity
{
public:
	Plane(float e) : Entity("Plane"),elasticVal(e) {}
	float elasticVal{200.f};
};

class ExtraScene : public Scene
{
	float mapSize{3000.f};
	float sponPoint{2500.f};
	std::vector<glm::vec3> particlePoint;
	Player* player;
	std::vector<Plane*> planes;
	std::vector<Entity*> blueBalls;
	std::vector<Entity*> redBalls;

	glm::mat4 proj;
	glm::mat4 view;
	glm::mat4 shadow;
	Entity* object;
	glm::mat4 shadowBias = { glm::vec4(0.5f,0.f,0.f,0.f),glm::vec4(0.f,0.5f,0.f,0.f),
	glm::vec4(0.f,0.f,0.5f,0.f) ,glm::vec4(0.5f,0.5f,0.5f,1.f) };
	glm::vec2 shadowMapSize{ 4096,4096 };

	const int numLight{ 1 };
	LightObj lightCam[2];
	unsigned int depthMapFBO[2]{};
	unsigned int depthMap[2]{};
	glm::mat3 lightCamPos{};

	bool behindShadow = true;
	int viewType{ 0 };
	float polygon_offset{};
	float polygon_units{};
	float border{ 0.f };

	float stTimeExplode{0.};
	const double explodeTMax{2.};
	double explodeT{0.};

	std::string shaderName{};

	void GravityBallSet();
	void PlaneSet();
	void lightCameraSet();

	void lightUpdate(double dt);
	void collisionSolve(double dt);
	void liftingBalls(double dt);

	void lightDrawSet();
	void lightCameraDraw();
public:
	ExtraScene(SceneManager& scm) : Scene(scm) {}

	void setup() override;
	void update(double dt) override;
	void ImguiUpdate(double dt) override;
	void draw() override;
	void unload() override;
	
	void MoveParticle();
};

inline bool isDetected(Entity* A,Entity* B)
{
	glm::vec3 AMin = A->GetPos() - glm::vec3{ A->GetScale().x / 2 ,A->GetScale().y / 2 ,A->GetScale().z / 2 };
	glm::vec3 AMax = A->GetPos() + glm::vec3{ A->GetScale().x / 2 ,A->GetScale().y / 2 ,A->GetScale().z / 2 };
	glm::vec3 BMIn = B->GetPos() - glm::vec3{ B->GetScale().x / 2 ,B->GetScale().y / 2 ,B->GetScale().z / 2 };
	glm::vec3 BMax = B->GetPos() + glm::vec3{ B->GetScale().x / 2 ,B->GetScale().y / 2 ,B->GetScale().z / 2 };


	return (AMin.x <= BMax.x && AMax.x >= BMIn.x) &&
		(AMin.y <= BMax.y && AMax.y >= BMIn.y) &&
		(AMin.z <= BMax.z && AMax.z >= BMIn.z);
}
inline bool isDetectedCircle(Entity* A, Entity* B)
{
	float Arad{A->GetScale().x/2.f};
	float Brad{ B->GetScale().x /2.f};
	float dist{glm::length(A->GetPos() - B->GetPos())};

	return dist <= (Arad + Brad);
}

inline glm::vec3 CRvec(double t, glm::vec3 P0, glm::vec3 P1, glm::vec3 P2, glm::vec3 P3)
{
	return 	P0 * (float)(-std::pow(t, 3) / 2 + t * t - t / 2)
		+ P1 * (float)((3 / 2 * std::pow(t, 3)) + (-5 / 2 * std::pow(t, 2)) + 1)
		+ P2 * (float)(-3 * std::pow(t, 3) / 2 + 2 * std::pow(t, 2) + t / 2)
		+ P3 * (float)(std::pow(t, 3) / 2 - std::pow(t, 2) / 2);

}