/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#pragma once
#include <string>
#include <glm/glm.hpp>

#include "../GraphicEngine/Model.h"


class Entity
{
public:
	Entity(std::string name, glm::vec3 pos = { 0,0,0 }, glm::vec3 scale = { 1,1,1 }, glm::vec3 rotate = {0,0,0});
	~Entity();

	virtual void Update(double dt);
	void Draw();
	void DrawBinding();// { model.DrawBinding(); }
	void DrawModel();// { model.DrawModel(); }
	void DrawUnBinding();// { model.DrawUnBinding(); }
	void SetShader(ShaderHandle* shaderHandle) { model.SetShader(shaderHandle); }
	ShaderHandle* getShaderhandle() { return model.getShader(); }
	MeshHandle* getMeshHandle() { return model.getMeshHandle(); }
	void SetMesh(MeshHandle* Mesh) { model.SetMesh(Mesh); }
	void AddUniform(uniform* uniform) { model.AddUniform(uniform); }
	bool FindUniform(std::string Name) {return model.FindUniform(Name); }
	void RemoveUniform(std::string Name) { model.RemoveUniform(Name); }
	void ClearUniform() { model.ClearUniform(); }
	glm::mat4& GetMatrix();
	
	 glm::vec3& GetPos()  { return pos; }
	 glm::vec3& GetVel()  { return vel; }
	 glm::vec3& GetScale()  { return scale; }
	 glm::vec3& GetRotate()  { return rotate; }
	 glm::vec4& GetColor()  { return color; }
	 std::string& getName() { return name; }
	
	void SetPos(glm::vec3 newPos);
	void SetVel(glm::vec3 newVel);
	void SetScale(glm::vec3 newScale);
	void SetRotate(glm::vec3 newRotate);
	void SetColor(glm::vec4 color);
	void SetDrawMode(int mode) { drawMode = mode; }
	void SetFaceMode(int mode) { faceMode = mode; }

	void UpdatePosition(glm::vec3 adjustPosition);
	void UpdateVelocity(glm::vec3 adjustVelocity);
	void UpdateRotation(glm::vec3 newRotationAmount);
	void UpdateColor(glm::vec4 adjustColor);
private:
	friend class Scene;
	Modeling model;
	glm::mat4 objectMatrix;
	glm::vec4 color;
	glm::vec3 pos{0,0,0};
	glm::vec3 vel{0,0,0};
	glm::vec3 scale{1,1,1};
	glm::vec3 rotate{0,0,0};
	int faceMode{ 0x0408 }; // GL_FRONT_AND_BACK
	int drawMode{ 0x1B02 }; // GL_FILL 

	bool updateMatrix;
	
	std::string name;
	
};
