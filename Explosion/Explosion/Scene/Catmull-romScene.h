/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#pragma once
#include <vector>
#include "../GraphicEngine/Mesh.h"
#include "../GraphicEngine/GraphicEngine.h"
#include "Scene.h"
#include "../src/Entity.h"
#include "../GraphicEngine/Function.h"
#include "Imgui/imgui.h"

struct CRPoint
{
	Entity* selfObj{nullptr};
	static constexpr float selfSize{50};
};
struct CRLine {
	Entity* lineObj{nullptr};
	std::string LineMeshName{};
	bool isChanged{false};
	std::vector<CRPoint> points;

	glm::vec3 getCRLinePoint(double t,bool isLoop = true);
	void UpdateCRLine();
};



inline glm::vec3 CR(double t, CRPoint P0, CRPoint P1, CRPoint P2, CRPoint P3)
{
	return 	P0.selfObj->GetPos() * (float)(-std::pow(t, 3) / 2 +  t  * t  - t / 2)
		+ P1.selfObj->GetPos() * (float)((3 / 2 * std::pow(t, 3)) + (-5 / 2 * std::pow(t, 2)) + 1)
		+ P2.selfObj->GetPos() * (float)(-3 * std::pow(t, 3) / 2 + 2 * std::pow(t, 2) + t / 2)
		+ P3.selfObj->GetPos() * (float)(std::pow(t, 3) / 2 - std::pow(t, 2) / 2);

}

class CatmullRomScene : public Scene
{
	std::vector<CRLine> CRLine{  };
	int selectedLineNum{ 0 };
public:
	CatmullRomScene(SceneManager& scm) : Scene(scm) {}

	void GenerateCRPoint(int lineNum, glm::vec3 pos, std::string name);
	void generateNewCRLine(glm::vec3 pos1, glm::vec3 pos2);
	void UpdateAllCRLines();

	void setup() override;
	void update(double dt) override;
	void ImguiUpdate(double dt) override;
	void draw() override;
	void unload() override;
	static unsigned int num;
	static int PointInLine;
	float lineWidth{5};
	Entity* backPlane{};

};



inline std::string GetCRLine(const int numOfvert, CRLine line)
{
	static int CRlineNum = 0;
	std::string meshName = std::to_string(numOfvert) + "AND" + std::to_string(CRlineNum++);
	/*if(meshName != End.ELineMeshName)
	{
		std::cout << meshName << " And " << End.ELineMeshName << " Not Matched \n";
	}*/
	std::vector<glm::vec3> pos_vtx{  };
	std::vector<GLuint> idx_vtx{ };
	for(int i = 0; i <= numOfvert * line.points.size(); i++)
	{
		glm::vec3 pos = line.getCRLinePoint(static_cast<float>(i) / static_cast<float>(numOfvert));
		pos_vtx.push_back(pos);
		idx_vtx.push_back(i);
	}
	GLuint vao_hdl;
	GLuint vbo_hdl;
	glCreateBuffers(1, &vbo_hdl);
	glNamedBufferStorage(vbo_hdl,
		sizeof(glm::vec3) * pos_vtx.size(), pos_vtx.data(), GL_DYNAMIC_STORAGE_BIT);
    glCreateVertexArrays(1, &vao_hdl);

	glEnableVertexArrayAttrib(vao_hdl, 0);
	glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec3));
	glVertexArrayAttribFormat(vao_hdl, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao_hdl, 0, 0);

	GLuint ebo_hdl;
	glCreateBuffers(1, &ebo_hdl);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_hdl);
	glNamedBufferStorage(ebo_hdl, sizeof(GLuint) * idx_vtx.size(),
		reinterpret_cast<GLvoid*>(idx_vtx.data()),
		GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
	glBindVertexArray(0);

	MeshHandle* m = new MeshHandle(meshName);
	m->vaoid = vao_hdl;
	m->draw_cnt = static_cast<GLuint>(idx_vtx.size());
	m->primitive_type = GL_LINE_STRIP;
	m->primitive_cnt = 0;//I don't know
	GraphicEngine::Meshs.insert({ meshName, m });
	return meshName;
}

inline void ChangeCRLine(std::string name, const int pointNum, CRLine line)
{
	std::string meshName = name;
	
	if (!EP::Mesh::ExistMesh(meshName))
	{
		std::cout << " Mesh not founded \n";
		return;
	}
	MeshHandle* old = EP::Mesh::GetMesh(name);
	
	std::vector<glm::vec3> pos_vtx{  };
	std::vector<GLuint> idx_vtx{ };
	for (int i = 0; i <= pointNum * line.points.size(); i++)
	{
		glm::vec3 pos = line.getCRLinePoint(static_cast<float>(i) / static_cast<float>(pointNum));
		pos_vtx.push_back(pos);
		idx_vtx.push_back(i);
	}
	GLuint vao_hdl = old->vaoid;
	GLuint vbo_hdl;
	//glDeleteBuffers(1, &old->vaoid);
	glCreateBuffers(1, &vbo_hdl);
	glNamedBufferStorage(vbo_hdl,
		sizeof(glm::vec3) * pos_vtx.size(), pos_vtx.data(), GL_DYNAMIC_STORAGE_BIT);
	//glCreateVertexArrays(1, &vao_hdl);

	glEnableVertexArrayAttrib(vao_hdl, 0);
	glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec3));
	glVertexArrayAttribFormat(vao_hdl, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao_hdl, 0, 0);

	GLuint ebo_hdl;
	glCreateBuffers(1, &ebo_hdl);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_hdl);
	glNamedBufferStorage(ebo_hdl, sizeof(GLuint) * idx_vtx.size(),
		reinterpret_cast<GLvoid*>(idx_vtx.data()),
		GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
	glBindVertexArray(0);

	old->vaoid = vao_hdl;
	old->draw_cnt = static_cast<GLuint>(idx_vtx.size());
	old->primitive_type = GL_LINE_STRIP;
	old->primitive_cnt = 0;//I don't know
	return;
}
