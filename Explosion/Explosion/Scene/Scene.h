/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#pragma once
#include <list>

#include "../src/Entity.h"
#include "../GraphicEngine/Shader.h"
#include "../GraphicEngine/Uniform.h"


class SceneManager;
class Scene {
	SceneManager& scm;
	std::list<Entity*> models;
	std::vector<uniform*> WorldUniform;
public:
	Scene(SceneManager& scm):scm(scm){}
	virtual void setup() {}
	virtual void update(double dt)
	{
		for (Entity* m : models)
		{
			m->Update(dt);
		}
	}
	virtual void ImguiUpdate(double /*dt*/) {}
	virtual void draw()
	{
		for(Entity* m : models)
		{
			m->DrawBinding();
			for(auto WU : WorldUniform)
			{
				WU->Set(m->model.shader->glslshader);
			}
			m->DrawModel();
			m->DrawUnBinding();
		}
	}
	virtual void unload()
	{
		for (Entity* m : models)
		{
			delete m;
		}
		for (uniform* WU : WorldUniform)
		{
			delete WU;
		}
		models.clear();
		WorldUniform.clear();
	}
	void AddModel(Entity* model) { models.push_back(model); }
	void AddWorldUniform(uniform* uni) { WorldUniform.push_back(uni); }
	std::list<Entity*>& getModels() { return models; }
};

