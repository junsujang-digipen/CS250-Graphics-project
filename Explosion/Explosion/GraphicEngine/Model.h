/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#pragma once
#include <memory>
#include <string>
#include <vector>


class ShaderHandle;
class MeshHandle;
class uniform;

class Modeling
{
public:
	Modeling(std::string name) : name(name), mesh(nullptr),shader(nullptr) {}
	~Modeling();
	void Draw();
	void DrawBinding();
	void DrawModel();
	void DrawUnBinding();
	void SetShader(ShaderHandle* shaderHandle);
	ShaderHandle* getShader() {return shader;}
	void SetMesh(MeshHandle* MeshHandle);
	void AddUniform(uniform* uniform);
	bool FindUniform(std::string name);
	void RemoveUniform(std::string name);
	void ClearUniform();
	MeshHandle* getMeshHandle() { return mesh; };
private:
	friend class Scene;
	std::string name;
	ShaderHandle* shader;
	MeshHandle* mesh;
	std::vector<uniform*> uniformList;
};