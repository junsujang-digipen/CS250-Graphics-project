/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#include "GraphicEngine.h"

#include <random>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mesh.h"
#include "Shader.h"

glm::ivec2 GraphicEngine::windowSize;
std::string GraphicEngine::titleName;
std::map<std::string, Viewport> GraphicEngine::viewports;
std::map<std::string, ShaderHandle*> GraphicEngine::shaders;
std::map<std::string, MeshHandle*> GraphicEngine::Meshs;

GraphicEngine::GraphicEngine():window(), deltaTime(0), timer(),windowPtr()
{
}

GraphicEngine::~GraphicEngine()
{
	
}


void GraphicEngine::Init(std::string title,glm::ivec2 screenSize)
{
    titleName = title;
    windowSize = screenSize;
	
    windowPtr = window.getWindowPtr();
    camera.init();
    glViewport(0, 0, (int)screenSize.x, (int)screenSize.y);

}

void GraphicEngine::Update()
{
    deltaTime =  glfwGetTime() - timer;
    SM.update(deltaTime);
    timer = (float)glfwGetTime();
    d += deltaTime;
    //std::cout << "timer: " << std::to_string(timer) << std::endl;
    //std::cout << "d: " << d << std::endl;
}


float GraphicEngine::GetTime()
{
    return static_cast<float>(glfwGetTime());
}

void GraphicEngine::Shutdown()
{
	for(auto sha : shaders)
	{
        delete sha.second;
	}
    for(auto me : Meshs)
    {
        delete me.second;
    }
    glfwDestroyWindow(windowPtr);
}

double GraphicEngine::RanGenerator(double min, double max)
{
    static std::knuth_b knuthrand(Seed);
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(knuthrand);

}
