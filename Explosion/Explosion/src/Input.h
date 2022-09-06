#pragma once
/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include <string>
#include <glm/glm.hpp>
#include <vector>

//class InputKey {
//public:
//	InputKey(int button);
//	void ReSetKey(int b);
//	bool IsKeyDown() const;
//	bool IsKeyReleased() const;
//	int get_key();
//private:
//	int button;
//};

class Keyboard {
	static std::map<int,bool> keyDown;
	static std::map<int,bool> wasKeyDown;
public:
	Keyboard();
	bool IskeyClicked(int key);
	bool IsKeyDown(int key) ;
	bool IsKeyReleased(int key) ;
	static void SetKeyDown(int key, bool value);
	static void Key_callback_func(GLFWwindow* pwin, int key, int scancode, int action, int mod);
	void update();
};

class Mouse {
	static std::map<int, bool> buttonDown;
	static std::map<int, bool> wasButtonDown;
	static glm::dvec2 mouseScrollVal;
	static glm::dvec2 mousePosition;
public:
	Mouse();
	bool IsButtonClicked(int key);
	bool IsButtonDown(int key);
	bool IsButtonReleased(int key);
	static void SetButtonDown(int button, bool value);
	glm::dvec2 getMouseScrollVal();
	glm::dvec2 getMousePosition();
	static void MouseButton_callback_func(GLFWwindow*, int, int, int);
	static void MouseScroll_callback_func(GLFWwindow*, double, double);
	static void MousePosition_callback_func(GLFWwindow*, double, double);
	void update();
};

class Input {
public:
	Input(GLFWwindow* );
	~Input();

	void Update();
	Keyboard getKeboard();
	Mouse getMouse();
private:
	GLFWwindow* winPtr;
	Keyboard keyBoard{};
	Mouse mouse{};
};