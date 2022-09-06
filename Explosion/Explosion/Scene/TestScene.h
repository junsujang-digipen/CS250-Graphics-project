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

class TestScene : public Scene
{
public:
	TestScene(SceneManager& scm) : Scene(scm) {}

	void setup() override;
	void update(double dt) override;
	void ImguiUpdate(double dt) override;
	void draw() override;
	void unload() override;

	Entity* object;

};