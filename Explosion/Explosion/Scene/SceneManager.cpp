/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

#include "SceneManager.h"
#include "Scene.h"
#include "../GraphicEngine/GraphicEngine.h"
SceneManager::SceneManager()
{
}

void SceneManager::putScene(Scene& sc, std::string&& scName)
{
	Scenes[scName] = &sc;
}

void SceneManager::init()
{
}

void SceneManager::update(double dt)
{
	switch (nowState)
	{
	case SceneManager::SceneManagerState::start:
		if (nextScene == nullptr) {
			nowState = SceneManagerState::Exit;
			//log error
		}
		else
		{
			nowState = SceneManagerState::Load;
		}
		break;
	case SceneManager::SceneManagerState::Load:
		prevScene = nowScene;
		nowScene = nextScene;
		nowScene->setup();
		nowState = SceneManagerState::Update;
		break;
	case SceneManager::SceneManagerState::Update:
		if (nowScene != nextScene) {
			nowState = SceneManagerState::Unload;
		}
		else {
			GraphicEngine::getCamera().Update(dt);
			nowScene->update(dt);
			
			nowScene->draw();
			nowScene->ImguiUpdate(dt);

		}
		

		break;
	case SceneManager::SceneManagerState::Unload:
		nowState = SceneManagerState::Load;
		nowScene->unload();
		break;
	case SceneManager::SceneManagerState::Exit:
		break;
	default:
		break;
	}
}

void SceneManager::setNextScene(std::string&& scName)
{
	if (Scenes.find(scName) == Scenes.end()) {
		//log error
	}
	nextScene = Scenes[scName];
}

void SceneManager::backToPrevScene()
{
	if (prevScene == nullptr) {
		//log error
		return;
	}
	nextScene = prevScene;
}
