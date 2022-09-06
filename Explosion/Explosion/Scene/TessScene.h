/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#pragma once
#include "Scene.h"
#include "../src/Entity.h"

class Modeling;

class TessScene : public Scene
{
public:
	TessScene(SceneManager& scm) : Scene(scm) {}

	void setup() override;
	void update(double dt) override;
	void ImguiUpdate(double dt) override;
	void draw() override;
	void unload() override;

	Entity* object;
	int tessInner{1};
	int tessOuter{1};
	float shrink{0.9f};
	bool isCircle{true};

	Entity* lightObj{};

	glm::vec3 windVec{0.f,0.f,0.f};
	glm::vec3 ambient{ 0.f,0.f,0.f };
	glm::vec3 materialCol{ 1.f,1.f,1.f };
	float shinvalue{ 10.f };
	int shaderType = 0;
};