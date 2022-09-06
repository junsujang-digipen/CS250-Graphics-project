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

struct HermitePoint
{
	glm::vec3 pos{};
	Entity* selfObj{};
	static constexpr float selfSize{50};
	static constexpr float VecSize{ 25 };

	glm::vec3 begin{};
	Entity* BVectorObj{};
	std::string BLineMeshName{};

	glm::vec3 end{};
	Entity* EVectorObj{};
	std::string ELineMeshName{};

	bool isChanged{false};
	void ChangePos(glm::vec3 p);
	void ChangeBVec(glm::vec3 v);
	void ChangeEVec(glm::vec3 v);
	void checkMouseOnSelf();
	bool checkMouseOnBeginVec();
	bool checkMouseOnEndVec();

	void updateBVecLine();
	void updateEVecLine();

};
struct HermiteLine {
	HermitePoint* begin{};
	HermitePoint* end{};
	std::string LineMeshName{};
	bool isChanged{false};
	void updateLineMesh();
	void updateLineMesh2();
	void ChangeMesh();
};


class HermiteScene : public Scene
{
	std::vector<HermitePoint*> hermitePoints{  };
	std::vector<HermiteLine*> hermiteLines{  };
	void GenerateHermitePoint(glm::vec3 pos, std::string name);
	void GenerateHermiteLine(HermitePoint* begin, HermitePoint* end);

public:
	HermiteScene(SceneManager& scm) : Scene(scm) {}

	void setup() override;
	void update(double dt) override;
	void ImguiUpdate(double dt) override;
	void draw() override;
	void unload() override;
	static unsigned int Lnum;
	static int PointInLine;
	float lineWidth{5};
	Entity* backPlane{};
private:
	float computeRT(glm::vec3 stPoint, glm::vec3 Rvector, Entity* obj);

};

inline glm::vec3 hermiteCurve(float t, HermitePoint begin, HermitePoint end)
{
	float h0 = (2*t*t*t) - (3*t*t) + 1;
	float h1 = (-2 * t * t * t) + (3 * t * t);
	float h2 = (t * t * t) - (2 * t * t) + t;
	float h3 = (t * t * t )- (t * t);

	glm::vec3 p0 = begin.pos;
	glm::vec3 p1 = end.pos;
	glm::vec3 v0 = begin.begin*10.f;
	glm::vec3 v1 = end.end*10.f;

	return h0 * p0 + h1 * p1 + h2 * v0 + h3 * v1;

}
inline int HermitLineNum = 0;
inline std::string GetHermitLine(const int numOfvert, HermitePoint Begin, HermitePoint End)
{
	std::string meshName = std::to_string(numOfvert) + "AND" + std::to_string(HermitLineNum++);
	if(meshName != End.ELineMeshName)
	{
		std::cout << meshName << " And " << End.ELineMeshName << " Not Matched \n";
	}
	std::vector<glm::vec3> pos_vtx{  };
	std::vector<GLuint> idx_vtx{ };
	for(int i = 0; i <= numOfvert; i++)
	{
		glm::vec3 pos = hermiteCurve(static_cast<float>(i) / static_cast<float>(numOfvert), Begin, End);
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
inline std::string GetLine(glm::vec3 posB, glm::vec3 posE, std::string n)
{
	std::string meshName = "Line" + n;
	std::vector<glm::vec3> pos_vtx{  };
	std::vector<GLuint> idx_vtx{ };

	pos_vtx.push_back(posB);
	pos_vtx.push_back(posE);
	idx_vtx.push_back(0);
	idx_vtx.push_back(1);

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

inline void ChangeHermiteCurve(std::string name, const int pointNum, HermitePoint* Begin, HermitePoint* End)
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
	for (int i = 0; i <= pointNum; i++)
	{
		glm::vec3 pos = hermiteCurve(static_cast<float>(i) / static_cast<float>(pointNum), *Begin, *End);
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

inline void ChangeLine(glm::vec3 posB, glm::vec3 posE, std::string n)
{
	std::string meshName = n;
	std::vector<glm::vec3> pos_vtx{  };
	std::vector<GLuint> idx_vtx{ };

	if (!EP::Mesh::ExistMesh(meshName))
	{
		std::cout << " Mesh not founded \n";
		return;
	}
	MeshHandle* old = EP::Mesh::GetMesh(meshName);
	pos_vtx.push_back(posB);
	pos_vtx.push_back(posE);
	idx_vtx.push_back(0);
	idx_vtx.push_back(1);


	GLuint vbo_hdl;
	glCreateBuffers(1, &vbo_hdl);
	glNamedBufferStorage(vbo_hdl,
		sizeof(glm::vec3) * pos_vtx.size(), pos_vtx.data(), GL_DYNAMIC_STORAGE_BIT);


	glEnableVertexArrayAttrib(old->vaoid, 0);
	glVertexArrayVertexBuffer(old->vaoid, 0, vbo_hdl, 0, sizeof(glm::vec3));
	glVertexArrayAttribFormat(old->vaoid, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(old->vaoid, 0, 0);

	GLuint ebo_hdl;
	glCreateBuffers(1, &ebo_hdl);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_hdl);
	glNamedBufferStorage(ebo_hdl, sizeof(GLuint) * idx_vtx.size(),
		reinterpret_cast<GLvoid*>(idx_vtx.data()),
		GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(old->vaoid, ebo_hdl);
	glBindVertexArray(0);

}
