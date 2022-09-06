/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#pragma once


#include <string>

struct GLFWwindow;
class Window {
	int width{};
	int height{};
	std::string title{};
	GLFWwindow* windowPtr{nullptr};
public:
	Window(int w= 600,int h = 600,std::string title = "No title");
	// todo size set func
	// todo title set func
	GLFWwindow* getWindowPtr() { return windowPtr; }
};