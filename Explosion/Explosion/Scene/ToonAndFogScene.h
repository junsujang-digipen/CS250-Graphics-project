/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#pragma once
#include <glm/vec4.hpp>

#include "Scene.h"
#include "../src/Entity.h"

class Modeling;

class ToonAndFogScene : public Scene
{
public:
	ToonAndFogScene(SceneManager& scm) : Scene(scm) {}

	void setup() override;
	void update(double dt) override;
	void draw() override;
	void unload() override;
	void ImguiUpdate(double);

	Entity* object{};
	Entity* lightObj{};

	 glm::vec3 ambient{0.f,0.f,0.f};
	 glm::vec3 materialCol{1.f,1.f,1.f};
	 glm::vec3 fColor{ 0.f,0.f,0.f };

	float fDensity{0.001f};
	float shinvalue{10.f};
};