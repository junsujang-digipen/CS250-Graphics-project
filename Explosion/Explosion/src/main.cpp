/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#include <Imgui/imgui.h>
#include <Imgui/imgui_impl_glfw.h>
#include <Imgui/imgui_impl_opengl3.h>

#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>


#include "../GraphicEngine/GraphicEngine.h"
#include "../Scene/SceneManager.h"
#include "../Scene/TestScene.h"
#include "../Scene/ToonAndFogScene.h"
#include "../Scene/Scene.h"
#include "Input.h"
#include "../Scene/NoiseScene.h"
#include "../Scene/HermiteScene.h"
#include "../Scene/Catmull-romScene.h"
#include "../Scene/GeometryScene.h"
#include "../Scene/TessScene.h"
#include "../Scene/FogWorldScene.h"
#include "../Scene/ShadowScene.h"
#include "../Scene/ExtraScene.h"
void imguiLoad();
void imguiUnload();
void imguiUpdate();
void imguiDraw();
//void errorCallBack(int error, char const* str);
static const std::string titleName{"Team Explosion Class Project" };
GLuint pgmHandle = 0;
GLuint vaoid = 0;

int main(void)
{

    GraphicEngine& g_engine = GraphicEngine::Instance();
    g_engine.Init(titleName, {1000, 800});
    Input input{ GraphicEngine::GetWindowPtr() };
    imguiLoad();

    TestScene testScene{ g_engine.GetSceneManager() };
    ToonAndFogScene ToonAndFogScene{ g_engine.GetSceneManager() };
    NoiseScene noiseScene{ g_engine.GetSceneManager() };
    HermiteScene hermiteScene{ g_engine.GetSceneManager() };
    CatmullRomScene catmullRoomScene(g_engine.GetSceneManager());
    GeometryScene geometryScene(g_engine.GetSceneManager());
    TessScene tessScene(g_engine.GetSceneManager());
    FogWorldScene fogWorld(g_engine.GetSceneManager());
    ShadowScene shadow(g_engine.GetSceneManager());
    ExtraScene extra(g_engine.GetSceneManager());

    g_engine.GetSceneManager().putScene(testScene,"Modeling");
    g_engine.GetSceneManager().putScene(ToonAndFogScene, "TOON&FOG");
    g_engine.GetSceneManager().putScene(fogWorld, "fogWorld");
    g_engine.GetSceneManager().putScene(noiseScene, "Noise");
    g_engine.GetSceneManager().putScene(hermiteScene, "Hermite");
    g_engine.GetSceneManager().putScene(catmullRoomScene, "CatmullRom");
    g_engine.GetSceneManager().putScene(geometryScene, "Geometry");
    g_engine.GetSceneManager().putScene(tessScene, "tess");
    g_engine.GetSceneManager().putScene(shadow, "shadow");
    g_engine.GetSceneManager().putScene(extra, "extra");
	g_engine.GetSceneManager().setNextScene("extra");

    

    while (!glfwWindowShouldClose(GraphicEngine::GetWindowPtr())) {

        //glfwMakeContextCurrent(GraphicEngine::GetWindowPtr());
        glfwGetWindowSize(GraphicEngine::GetWindowPtr(), &GraphicEngine::windowSize.x, &GraphicEngine::windowSize.y);
    	glViewport(0, 0, GraphicEngine::windowSize.x, GraphicEngine::windowSize.y);
        glClearColor(0.0f, 0.0f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        imguiUpdate();
        g_engine.Update();
        imguiDraw();
        
        glfwSwapBuffers(GraphicEngine::GetWindowPtr());
        glfwPollEvents();
    }

    imguiUnload();
    glfwDestroyWindow(GraphicEngine::GetWindowPtr());
    
	return 0;
}

void imguiLoad()
{
    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
  
    // Setup Platform/Renderer bindings
    ImGui::StyleColorsDark();
    const char* glsl_version = "#version 330";
	ImGui_ImplGlfw_InitForOpenGL(GraphicEngine::GetWindowPtr(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void imguiUnload()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


bool show_sceneChange_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
void imguiUpdate()
{
    // feed inputs to dear imgui, start new frame
	
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
        if(ImGui::BeginMainMenuBar())
        {
            if(ImGui::BeginMenu("Scene"))
            {
                if (ImGui::MenuItem("Modeling")) {
                    GraphicEngine::GetSceneManager().setNextScene("Modeling");
                }
                if (ImGui::MenuItem("TOON&FOG")) {
                    GraphicEngine::GetSceneManager().setNextScene("TOON&FOG");
                }
                if (ImGui::MenuItem("fogWorld")) {
                    GraphicEngine::GetSceneManager().setNextScene("fogWorld");
                }
                if (ImGui::MenuItem("Noise")) {
                    GraphicEngine::GetSceneManager().setNextScene("Noise");
                }
                if (ImGui::MenuItem("Hermite")) {
                    GraphicEngine::GetSceneManager().setNextScene("Hermite");
                }
                if (ImGui::MenuItem("CatmullRom")) {
                    GraphicEngine::GetSceneManager().setNextScene("CatmullRom");
                }
                if (ImGui::MenuItem("Geometry")) {
                    GraphicEngine::GetSceneManager().setNextScene("Geometry");
                }
                if (ImGui::MenuItem("tess")) {
                    GraphicEngine::GetSceneManager().setNextScene("tess");
                }
                if (ImGui::MenuItem("shadow")) {
                    GraphicEngine::GetSceneManager().setNextScene("shadow");
                }
                if (ImGui::MenuItem("extra")) {
                    GraphicEngine::GetSceneManager().setNextScene("extra");
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

}
void imguiDraw()
{
    // Render dear imgui into screen
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}