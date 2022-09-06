/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#include <iostream>
#include "Window.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void errorCallBack(int error, char const* str);


Window::Window(int w, int h, std::string title):width(w),height(h),title(title)
{
    title = { "Team Explosion Class Project" };
    glfwMakeContextCurrent(NULL);
	if (!glfwInit())
	{
		std::cout << "GLFW init Failed";
		return;
	}
	glfwSetErrorCallback(errorCallBack);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    windowPtr = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!windowPtr)
    {
        std::cout << "GLFW can't make window";
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(windowPtr);
    glfwSwapInterval(1);

    //glfwSetFramebufferSizeCallback(windowPtr, fbsize_cb);
    //glfwSetKeyCallback(windowPtr, key_cb);
    //glfwSetMouseButtonCallback(windowPtr, mousebutton_cb);
    //glfwSetCursorPosCallback(windowPtr, mousepos_cb);
    //glfwSetScrollCallback(windowPtr, mousescroll_cb);

    glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Unable to initialize GLEW - error: "
            << glewGetErrorString(err) << " abort program" << std::endl;
        return;
    }
    if (GLEW_VERSION_4_5) {
        std::cout << "Using glew version: " << glewGetString(GLEW_VERSION) << std::endl;
        std::cout << "Driver supports OpenGL 4.5\n" << std::endl;
    }
    else {
        std::cerr << "Driver doesn't support OpenGL 4.5 - abort program" << std::endl;
        return;
    }
}


void errorCallBack(int error, char const* str)
{
    std::cerr << "GLFW " << error << " error: " << str << std::endl;
}