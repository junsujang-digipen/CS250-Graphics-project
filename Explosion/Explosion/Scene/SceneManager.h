/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#pragma once

#include <map>
#include <string>
class Scene;
class SceneManager {

	enum class SceneManagerState {
		start,
		Load,
		Update,
		Unload,
		Exit,
	};

	std::map<std::string,Scene*> Scenes{};
	Scene* nowScene{};
	Scene* nextScene{};
	Scene* prevScene{};
	SceneManagerState nowState{};

public:
	SceneManager();
	void putScene(Scene& sc, std::string&& scName);
	void init();
	void update(double dt);
	void UpdateIMGUI(double dt);
	void setNextScene(std::string&& scName);
	void backToPrevScene();
};