/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#pragma once
#include <string>
#include "glslshader.h"
#include "../src/Window.h"
#include "../Scene/SceneManager.h"
#include "../GraphicEngine/Camera.h"

/// <summary>
/// Set the Opengl(glew, glfw)
/// </summary>
///
struct GLFWwindow;
class ShaderHandle;
class MeshHandle;
class Modeling;

class Viewport
{
public:
	glm::ivec2 pos;
	glm::ivec2 size;

	Viewport(glm::ivec2 position, glm::ivec2 siz) : pos(position), size(siz) {}
	void Draw() { glViewport(pos.x, pos.y, size.x, size.y); }

};
class GraphicEngine
{
public:
	static GraphicEngine& Instance()
	{
		static GraphicEngine instance{};
		return instance;
	}

	void Init(std::string title, glm::ivec2 screenSize = glm::ivec2{600,400});
	void Update();
	void Shutdown();
private:

	GraphicEngine();
	~GraphicEngine();
	Window window;
	Camera camera{};
	SceneManager SM;
	GLFWwindow* windowPtr;
	double deltaTime{0};
	double timer{0};
	double d{0};
	struct GLViewport {
		GLint x, y;
		GLsizei width, height;
	};

public: // helper
	static float GetTime();
	static GLFWwindow* GetWindowPtr() { return Instance().windowPtr; }
	static Camera& getCamera() { return Instance().camera; }
	static SceneManager& GetSceneManager() { return Instance().SM; }
	static glm::ivec2 windowSize;
	static std::string titleName;
	static std::vector<GLViewport> vps; // container for viewports
	static double GetRandomNum(double min, double max) { return Instance().RanGenerator(min, max); }

	static std::map<std::string, Viewport> viewports;
	static std::map<std::string, ShaderHandle*> shaders;
	static std::map<std::string, MeshHandle*> Meshs;
	static constexpr float PI{ 3.14159f };
private:
	unsigned int Seed{};
	double RanGenerator(double min, double max);
};










