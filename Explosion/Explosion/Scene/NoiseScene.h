/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

#pragma once

//#include <glm/vec4.hpp>

#include "Scene.h"
#include "../src/Entity.h"

class Modeling;

class NoiseScene : public Scene
{
	static constexpr int width = 512;
	static constexpr int height = 512;
	//float* noiseMap{ new float[width * height] };
	float frequency = 0.05f;
	const float PI{ 3.14f };
	float seed{10.f};
	float size{512.f};
	float freq{0.01f};
	int type{0};
	int numLayer{5};
	float frequencyMult{2.f};
	float amplitude{0.8f};
	float amplitudeMult{0.35f};
	bool isAnimation{false};
	float xSpeed{}, ySpeed{},zSpeed{1.f};
	bool isPlaneView{ false };
	int planeviewTemp{ isPlaneView };
	float time{};
	enum class Method {
		Shader = 0,
		Value,
		Perlin,
	};
	Method noiseMth{ Method::Perlin };
	static constexpr int noiseSize{256};
	static constexpr int noiseArrSize{ noiseSize* noiseSize };
	


	GLubyte* imageData = 0;
	GLSLShader* texShdr{};
	GLuint textureId{};
	GLuint pboIds{};
	//GLuint noiseArray[noiseArrSize]{};
	void noiseDataBind();
	void pboGene();
	void pboDele();
public:
	NoiseScene(SceneManager& scm) : Scene(scm) {}
	
	void setup() override;
	void update(double dt) override;
	void draw() override;
	void unload() override;
	void ImguiUpdate(double);

	Entity* object{};
	Entity* backPlane{};
	std::vector<Entity*> xyzAxis{};
};